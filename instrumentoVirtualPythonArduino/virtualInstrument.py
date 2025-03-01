#!/usr/bin/env python
# -*- coding: utf-8 -*-
"""
@author: carlos xavier rosero

"""
from __future__ import division
import numpy
import serial, struct
from collections import deque
import time
import csv


import matplotlib.pyplot as plt
import matplotlib.animation as animation


class adapt(serial.Serial):
    def write(self, data):
        super(self.__class__, self).write(str(data))

class serialCx:

    def __init__(self, usbPort, frameLen, maxLen, EOL, bauds):

        self.ay1 = deque([0.0]*maxLen) #create buffer for plotting
        self.ay2 = deque([0.0]*maxLen)
        self.ay3 = deque([0.0]*maxLen)
    
        self.maxLen = maxLen            
        self.eol = bytearray(EOL.encode()) #end of line                       
        self.lenEol = len(self.eol) #length of the end of line    
        self.frameLen = frameLen - self.lenEol    
        self.timE = int(0)
        self.value1 = float(0) 
        self.value2 = float(0) 
            
        # open serial port
        
        print ("Establishing connection to serial port", usbPort)
       
        try:
            self.ser = adapt(port=usbPort, baudrate=bauds, timeout=1, parity=serial.PARITY_NONE,
                             stopbits=serial.STOPBITS_ONE)

            self.ser.open
            self.ser.isOpen
            
            inputS = bytearray() #initialize buffer
            while self.ser.inWaiting() > 0: #neglect the trash code received
                a = self.ser.read(1)
                a = int.from_bytes(a,"big")
                inputS.append(a) #append a new value into the buffer
          
            print('Connection established!')
                                
        except:
            print('Connection flopped!')
            print('Aborting...')



    def logging(self, timE, val1, val2):
        
        with open('logFile.csv', mode='a', newline='') as logFile:
            logFile = csv.writer(logFile, delimiter=',', quotechar='"',
                                 quoting=csv.QUOTE_MINIMAL)
            
            logFile.writerow([str(timE), str(val1), str(val2)])
            
  
    def addToBuf(self, buf, val):
        if len(buf) < self.maxLen:
            buf.append(val)
        else:
            buf.pop()
            buf.appendleft(val)

    # update plot
    def update(self, frameNum, a1, a2):
        try:
            line = bytearray()
            while True:
                c = self.ser.read(1)
                d = int.from_bytes(c,"big")

                if c:
                    line.append(d)
                    
                    if line[-self.lenEol:] == self.eol:
                        line = line[0:-self.lenEol] #removes EOF
                        break
                else:
                    break
                
            if len(line) == self.frameLen:
                # data = [int(val) for val in line[0:-4]] #except the last 4 elements

                # time = (data[0]<<24 | data[1] << 16 | data[2] << 8 | data[3])*self.factorFCY

                self.timE = struct.unpack('I', bytes(line[0:4]))
                self.timE = self.timE[0]  #takes out the only one element from the tuple
                
                self.value1 = struct.unpack('f', bytes(line[4:8]))
                self.value1 = self.value1[0] #takes out the only one element from the tuple

                self.value2 = struct.unpack('f', bytes(line[8:12]))
                self.value2 = self.value2[0] #takes out the only one element from the tuple

                #print(self.timE, self.value1, self.value2)

                # add data to buffer
                self.addToBuf(self.ay1, self.value1)
                self.addToBuf(self.ay2, self.value2)
                
                self.logging(self.timE, self.value1, self.value2)
                
                a1.set_data(range(self.maxLen), self.ay1)
                a2.set_data(range(self.maxLen), self.ay2)



            else:
                #print('Incorrect frame length:',len(line))
                self.ser.flushInput() #gets empty the serial port buffer

        except KeyboardInterrupt:
            print('Exiting...')


    # clean up
    def close(self):
        # close serial
                                                                     
        self.ser.flushInput()
        self.ser.close()

def plotting(givenData):

    fig = plt.figure()
    ax1 = plt.subplot(211)
    ax1.grid(True)
    a1, = ax1.plot([0], [0], color=(1, 0, 0))
    plt.xlim(0, 200)
    plt.ylim(-10, 10)
    plt.ylabel('Temperature')
    plt.title('Plot1', fontsize=20)

    ax2 = plt.subplot(212)
    ax2.grid(True)
    a2, = ax2.plot([], [], color=(0, 0.5, 0.5))
    plt.xlim(0, 200)
    plt.ylim(-10, 10)
    plt.ylabel('Pressure')
    plt.title('Plot 2', fontsize=20)


    anim = animation.FuncAnimation(fig, givenData.update, fargs=(a1, a2), interval=50)

    plt.show() # show plot

#######################################
######here starts the main program#####
#######################################
if __name__ == "__main__":

    testData = serialCx('COM6', 16, 200, 'aBcD', 115200)
    plotting(testData)
    serialCx.close(testData)
