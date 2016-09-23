import sys,time,os, re
from datetime import date
from time import sleep
sockdir="/home/cmspixel/TriDAS/pixel/BPixelTools/tools/python"
if not sockdir in sys.path: sys.path.append(sockdir)
from SimpleSocket import SimpleSocket
import datetime
import time


########################################################


# configuration
fechost = 'localhost'      
fecport =  2001  


fedslot =  99
fed=0                      # not used
pxfec=2000                 
caen=0                     # not used
fedport =  2006
########################################################


for i in range(0,48,1):


  # connect
  print "connecting to pxfec,",
  pxfec=SimpleSocket( 'localhost', 2015)
  print " done"
  
  print "connecting to ccu,",
  ccu=SimpleSocket( 'localhost', 2005)
  print " done"
  
  print "connecting to digitalfed,",
  digfed=SimpleSocket( 'localhost', 2060)
  print " done"
  
  
  
  
  
#import actual time
  ts = time.time()
#st = datetime.datetime.fromtimestamp(ts).strftime('pll_newFW_ChannelAB_%d-%m-%Y_%H:%M:%S.txt')
  st = "{0:d}".format(i)



#variables to play with
  NrOfBursts = 1000
  hertz = 40                   #in [kHz]
  TriggerPerBurst = 100
  gain = 2
  bias = 30
  
  
  rate = int(1./(hertz *25*10**(-6)))
  if (rate>65535):
    rate = 65536
    print "Warning: Rate set too high, set it on 65536 "
    
  print rate
  digfed.send("rate {0:d}".format(rate)).readlines()
  digfed.send("burst {0:d}".format(TriggerPerBurst)).readlines()
  digfed.send("confamc").readlines()



#create a textfile
  datafile = open(st, 'w')     # where 'datafile' is the name of the created text file (take care, this will
                             # overwrite the existing file!!)




#initialize poh on gain=0 and bias=0
  ccu.send("channel 11").readlines()
  ccu.send("poh2 set g0 {0:d}".format(gain)).readlines()
  ccu.send("poh2 set b0 {0:d}".format(bias)).readlines()


#initialize module (tbm, pll,...) with help of pxfec
  pxfec.send("mask").readlines()
  pxfec.send("tbm disable triggers").readlines()
  pxfec.send("module 31").readlines()
  pxfec.send("tbm").readlines()
  pxfec.send("tbmplldelay 0").readlines()   # set the delay setting on 0


#setting title with function in digitalfed.cpp
  datafile.write("#the gain was set to: " )
  datafile.write("{0:d}".format(gain))
  datafile.write('\t')
  datafile.write("the bias was set to: ")
  datafile.write("{0:d}".format(bias))
  datafile.write('\n')

  for i in digfed.send("titlepll {0:d}".format(NrOfBursts)).readlines():
    print i
    datafile.write(i)
    datafile.write('\n')




  for delay in range(0,64):            # loop over all possible pll delay settings
            
    inputo= str(bin(delay))+str(11)    # since 64 are just 6 bits, but we need 8 (where the last 2 are just space holder)
    inputo= int(inputo,2)              # we need to add by hand two '1' at the end of the binary string
          
    print "inputo " +str(inputo)       # print the new number (with the two '1' in the back) as a binary string
    pxfec.send("tbmplldelay "+ str(inputo)).readlines()     # send the number as a command to set the delay settings
    sleep(7)                                                # tbm needs this time to set its new value

 # sleep(1)
 # pxfec.send("tbm reset tbm").readlines()
 # sleep(1)



#  for i in range(0, 10):
#    digfed.send("testab 100").readlines()

    bdelay=bin(delay)

    if len(bdelay)>5:
      x=bdelay[-3:]                    # last 3 elements -> give us value for 400MHz phase adjust (0-7)
      y=bdelay[:-3]                    # first 3 elements -> give us value for 160MHz phase adjust (0-7)

    if len(bdelay) <=5:                 # y is zero therefore we have only 5 bits (3 of 'x' and the two '1')
      x=bdelay
      y=bin(0)

    
    x = int(x, 2)
    y = int(y, 2)

    print "begin with testprogram for ",str(y), "and ", str(x)

    for l in digfed.send("testab {0:d}".format(NrOfBursts)).readlines():
      print l
      datafile.write("{0:d}".format(y))
      datafile.write("\t")
      datafile.write("{0:d}".format(x))
      datafile.write("\t")
      datafile.write(l)
      datafile.write('\n')

    print "finished"



  ccu.close()
  digfed.close()
  pxfec.close()
