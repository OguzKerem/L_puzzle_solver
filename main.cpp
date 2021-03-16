/*
 * hello_world.cpp
 *
 *  Created on: Mar 15, 2021
 *      Author: oguzkerem
 */

/*
 * 1. Open up the project properties
 * 2. Visit C/C++ General > Preprocessor Include Paths, Macros, etc
 * 3. Select the Providers tab
 * 4. Check the box for "CDT GCC Built-in Compiler Settings"
 * 5. Set the compiler spec command to "xtensa-esp32-elf-gcc ${FLAGS} -E -P -v -dD "${INPUTS}""
 * 6. Rebuild the index
*/
#include <stdio.h>
#include <string.h>
#include "esp_wifi.h"
#include "esp_log.h"
#include "esp_wifi_types.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_console.h"
#include "esp_vfs_dev.h"
#include "driver/uart.h"
#include "sdkconfig.h"
#include <iostream>
#include "Nodes.h"
#include "bfs.h"
#include <regex>
#include <string>
#include <algorithm>
#include <iterator>

enum solverState{
	SOLVER_IDLE = 0,
	SOLVER_LOADING_PUZZLE,
	SOLVER_LOADED,
	SOLVER_SOLVING_PUZZLE,
};

extern "C" {
	int app_main(void);
}

static void initialize_console(void)
{
    /* Drain stdout before reconfiguring it */
    fflush(stdout);
    fsync(fileno(stdout));

    /* Disable buffering on stdin */
    setvbuf(stdin, NULL, _IONBF, 0);

    /* Minicom, screen, idf_monitor send CR when ENTER key is pressed */
    esp_vfs_dev_uart_port_set_rx_line_endings(CONFIG_ESP_CONSOLE_UART_NUM, ESP_LINE_ENDINGS_CR);
    /* Move the caret to the beginning of the next line on '\n' */
    esp_vfs_dev_uart_port_set_tx_line_endings(CONFIG_ESP_CONSOLE_UART_NUM, ESP_LINE_ENDINGS_CRLF);

    /* Configure UART. Note that REF_TICK is used so that the baud rate remains
     * correct while APB frequency is changing in light sleep mode.
     */
    uart_config_t uart_config = {0};
    uart_config.baud_rate = 115200;
    uart_config.data_bits = UART_DATA_8_BITS;
    uart_config.parity = UART_PARITY_DISABLE;
    uart_config.stop_bits = UART_STOP_BITS_1;

    /* Install UART driver for interrupt-driven reads and writes */
    ESP_ERROR_CHECK( uart_driver_install(CONFIG_ESP_CONSOLE_UART_NUM, 256, 0, 0, NULL, 0) );
    ESP_ERROR_CHECK( uart_param_config(CONFIG_ESP_CONSOLE_UART_NUM, &uart_config) );

    /* Tell VFS to use UART driver */
    esp_vfs_dev_uart_use_driver(CONFIG_ESP_CONSOLE_UART_NUM);

    /* Initialize the console */
    esp_console_config_t console_config = {};
    console_config.max_cmdline_args = 8;
    console_config.max_cmdline_length = 256;

    ESP_ERROR_CHECK( esp_console_init(&console_config) );
}

int uart_parser(string &message)
{
    static int i = 0;
    char ch;
    const char *delp;

    int byteRead;
    static char line[SIZE_OF_PUZZLE*2]; // TODO: make dynamic for larger puzzles
    static string puzzleStr;
    int size = 1;
    do{
        byteRead = uart_read_bytes(CONFIG_ESP_CONSOLE_UART_NUM, (uint8_t*)&ch, size, (10000 / 1000) / portTICK_RATE_MS); //TODO: Value of timeout value!
        vTaskDelay(pdMS_TO_TICKS(1));
    }while(byteRead < 1);

    if(ch != 0x0D){
    	puzzleStr.push_back(ch);
		line[i++] = ch;
		cout << ch << flush;
	}else{
		delp = strtok (line, " ");
		int inputCnt = 0;
		while (delp != NULL)  {
			delp = strtok (NULL, " ");
			inputCnt++;
		}
		if(inputCnt == SIZE_OF_PUZZLE){
			message = puzzleStr;
			i = 0;
			return SOLVER_LOADED;
		}else{
			i = 0;
			inputCnt = 0;
			delp = NULL;
			cout << endl;
		}

	}

    return SOLVER_LOADING_PUZZLE;
}
int app_main(void)
{

	int finalState[SIZE_OF_PUZZLE] = 	{
											1,2,3,
											4,5,6,
											7,8,0
										};
	int puzzleArray[SIZE_OF_PUZZLE];
	int puzzleArrayIndex = 0;
	string puzzle;
	initialize_console();

	int solverState = 0;

	regex reg("\\s+");

	    while (true) {
	        /*
	         * The line is returned when ENTER is pressed.
	         */
	    	switch(solverState)
	    	{
	    	case SOLVER_IDLE:
	    		cout << "PLEASE ENTER YOUR PUZZLE: \n";
	    		solverState = SOLVER_LOADING_PUZZLE;
	    		break;
				case SOLVER_LOADING_PUZZLE:
					solverState = uart_parser(puzzle);
					vTaskDelay(pdMS_TO_TICKS(10));
					break;
				case SOLVER_LOADED:
				{
					//tokenize input string with " "

					sregex_token_iterator iter(puzzle.begin(), puzzle.end(), reg, -1);
					sregex_token_iterator end;
					vector<string> vec(iter, end);

					//convert string to integer array
					for (auto a : vec)
					{
						std::string::size_type sz;   // alias of size_t
						puzzleArray[puzzleArrayIndex++] = std::stoi (a,&sz);
					}

					//check input puzzle elements are in goal state as well
					int isPuzzleValidCnt = 0;
					for(int i = 0; i < SIZE_OF_PUZZLE; i++){
						for(int j = 0; j < SIZE_OF_PUZZLE; j++){
							if(puzzleArray[i] == finalState[j]){
								isPuzzleValidCnt++;
							}
						}
					}

					if(isPuzzleValidCnt == SIZE_OF_PUZZLE){
						cout << "\nPUZZLE LOADED SUCCESFULLY\n";
						solverState = SOLVER_SOLVING_PUZZLE;
					}else{
						//TODO:there are somethings need to be clean

						cout << "\nPUZZLE DOES NOT MATCH WITH GOAL STATE\n";
						cout << "PLEASE ENTER A NEW PUZZLE\n";
						solverState = SOLVER_IDLE;
					}

					break;
				}
				case SOLVER_SOLVING_PUZZLE:{


					Nodes firstNode(puzzleArray, finalState, NULL, 0);

					cout << "YOUR PUZZLE IS:\n";
					firstNode.printPuzzle();

					list<Nodes*> solutionPath;
					bfs search;
					bool solutionFound = false;
					solutionPath = search.executeBfs(&firstNode, &solutionFound);
					if(solutionFound == false){
						cout << "NO SOLUTION IN DEPTH 4th\n";
						return 0;
					}else{
						reverse(solutionPath.begin(), solutionPath.end());
						for (auto i : solutionPath) {
						    i->printPuzzle();
						    cout << endl;
						  }
					}

					return 0;
				}

					break;
				default:
					break;
	    	};

	    }


//	int startPoint[SIZE_OF_PUZZLE] = {0,2,1,
//									  4,5,3,
//									  7,8,6};

//	int startPoint[SIZE_OF_PUZZLE] = {0,1,2,
//								      4,5,3,
//									  7,8,6};

/*
	int startPoint[SIZE_OF_PUZZLE] = {1,2,3,
									  4,5,6,
									  0,7,8};


	Nodes firstNode(startPoint, NULL, 0);


	list<Nodes*> solutionPath;
	bfs search;
	bool solutionFound = false;
	solutionPath = search.executeBfs(&firstNode, &solutionFound);
	if(solutionFound == false){
		printf("NO SOLUTION IN DEPTH 4th\n");
	}else{
		int i = 0;
		cout << "solutionPath contains:";
		for (auto it=solutionPath.begin(); it != solutionPath.end(); ++it)
			printf("path: %d\n", i++);
		cout << "\n";
	}
	printf("-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-\n");
	return 0;
*/
}








