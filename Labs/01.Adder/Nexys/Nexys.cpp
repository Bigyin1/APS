// SPDX-License-Identifier: Apache-2.0
// Copyright 2021 Olof Kindgren
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
// http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include <iostream>
#include "Nexys.hpp"





// void sendToSevenSegByNum(vidbo_context_t *vidbo_context, int num, int val) {

//   char item2[13] = {0};

//   if(val < 10)
//     val = val + 48; // to ASCII
//   else
//     val = val + 55;

//   snprintf(item2, 13, "SevSegDispl%d", num);

//   vidbo_send(vidbo_context, main_time, item2, "digit", val);
// }


void processSevSeg(int vidbo_context, Vnexys_adder* top) {
  
  static vluint64_t lastSegSend = 0;
  static int lastDigit[8] = {-1};

  int Digit = (top->CA<<6)+(top->CB<<5)+(top->CC<<4)+(top->CD<<3)+(top->CE<<2)+(top->CF<<1)+(top->CG);

  int Digit_Hex = 0;

  if (Digit == 0x01) Digit_Hex = 0;
  if (Digit == 0x4f) Digit_Hex = 1;
  if (Digit == 0x12) Digit_Hex = 2;
  if (Digit == 0x06) Digit_Hex = 3;
  if (Digit == 0x4c) Digit_Hex = 4;
  if (Digit == 0x24) Digit_Hex = 5;
  if (Digit == 0x20) Digit_Hex = 6;
  if (Digit == 0x0f) Digit_Hex = 7;
  if (Digit == 0x00) Digit_Hex = 8;
  if (Digit == 0x04) Digit_Hex = 9;
  if (Digit == 0x08) Digit_Hex = 10;
  if (Digit == 0x60) Digit_Hex = 11;
  if (Digit == 0x72) Digit_Hex = 12;
  if (Digit == 0x42) Digit_Hex = 13;
  if (Digit == 0x30) Digit_Hex = 14;
  if (Digit == 0x38) Digit_Hex = 15;

  int digIdx = 0;
  switch (top->AN)
  {
    case 0xFE:
      digIdx = 0;
      break;
    case 0xFD:
      digIdx = 1;
      break;
    case 0xFB:
      digIdx = 2;
      break;
    case 0xF7:
      digIdx = 3;
      break;
    case 0xEF:
      digIdx = 4;
      break;
    case 0xDF:
      digIdx = 5;
      break; 
    case 0xBF:
      digIdx = 6;
      break;
    case 0x7F:
      digIdx = 7;
      break;
    default:
      break;
  }

  lastDigit[digIdx] = Digit_Hex;
  
  if (main_time - lastSegSend >= 1500043) {

    for (size_t i = 0; i < 8; i++)
      sendToSevenSegByNum(vidbo_context, i, lastDigit[i]);

    lastSegSend = main_time;
  }
}


int main(int argc, char **argv, char **env) {

  const char * const inputs[] = {
    "gpio.SW0",
    "gpio.SW1",
    "gpio.SW2",
    "gpio.SW3",
    "gpio.SW4",
    "gpio.SW5",
    "gpio.SW6",
    "gpio.SW7",
    "gpio.SW8",
    "gpio.SW9",
    "gpio.SW10",
    "gpio.SW11",
    "gpio.SW12",
    "gpio.SW13",
    "gpio.SW14",
    "gpio.SW15",
    "gpio.SW16",
    "gpio.SW17",
    "gpio.SW18",
    "gpio.SW19",
    "gpio.SW20",
  };
  int num_inputs = sizeof(inputs) / sizeof(inputs[0]);

  int *input_vals = (int *)calloc(num_inputs, sizeof(int));

  vidbo_init(&vidbo_context, 8081);

  vidbo_register_inputs(&vidbo_context, inputs, num_inputs);
  
  Verilated::commandArgs(argc, argv);

  Vnexys_adder* top = new Vnexys_adder;

  int check_vidbo = 0;

  printf("\nHELLO_TB\n");

  top->CLK100 = 1;
  int last_leds = top->LED;
  int sidx = 0;


  while (!(done || Verilated::gotFinish())) {

    top->eval();

    check_vidbo++;

    char item[5] = {0}; //Space for LD??\0

    printf("in loop\n");
    if (last_leds != top->LED) {
        for (int i=0;i<16;i++) {
          snprintf(item, 5, "LD%d", i);
          vidbo_send(&vidbo_context, main_time, "gpio", item, (top->LED>>i) & 0x1);
        }
        last_leds = top->LED;
    }

    if (vidbo_recv(&vidbo_context, input_vals)) {
      /* Update the GPIO inputs from the received frame */
      top->SW = 0;
      for (int i=0;i<16;i++)
        if (input_vals[i])
          top->SW |= (1 << i);
    }


    if (top->CLK100 == 0) { 
      processSevSeg(&vidbo_context, top);
    }

    top->CLK100 = !top->CLK100;
    main_time+=10;

    top->resetn = 1;
    if (main_time <= 100)
      top->resetn = 0;
  }

  exit(0);
}


void Nexys::Start() {

  started.store(true);

  top.CLK100 = 1;

  while (!Verilated::gotFinish())
  {
    top.eval();

    top.resetn = 1;
    if (mainTime <= 100)
      top.resetn = 0;

    


    tick();
  }
  

  
}


void Nexys::tick() {
  top.CLK100 = !top.CLK100;
  mainTime+=10;
}

void Nexys::reset() {
  top.resetn = 0;
}
