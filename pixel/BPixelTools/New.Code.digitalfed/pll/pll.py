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
st = datetime.datetime.fromtimestamp(ts).strftime('pll_newFW_spamon_%d-%m-%Y_%H:%M:%S.txt')





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

print "1"

#create a textfile
datafile = open(st, 'w')     # where 'datafile' is the name of the created text file (take care, this will
                             # overwrite the existing file!!)




#initialize poh on gain=0 and bias=0
ccu.send("channel 11").readlines()
ccu.send("poh2 set g0 {0:d}".format(gain)).readlines()
#sleep(0.1)
ccu.send("poh2 set b0 {0:d}".format(bias)).readlines()
#sleep(0.1)
print "2"

#initialize module (tbm, pll,...) with help of pxfec
pxfec.send("mask").readlines()
sleep(0.1)
pxfec.send("tbm disable triggers").readlines()
sleep(0.1)
pxfec.send("module 31").readlines()
sleep(0.1)
pxfec.send("tbm").readlines()
sleep(0.1)
pxfec.send("tbmplldelay 0").readlines()
sleep(0.1)  # set the delay setting on 0
#pxfec.send("tbmplldelay 211").readlines() 
sleep(0.1)
print "3"
#setting title with function in digitalfed.cpp
datafile.write("#the gain was set to: " )
sleep(0.1)
datafile.write("{0:d}".format(gain))
sleep(0.1)
datafile.write('\t')
sleep(0.1)
datafile.write("the bias was set to: ")
sleep(0.1)
datafile.write("{0:d}".format(bias))
sleep(0.1)
datafile.write('\n')
for i in digfed.send("titlepll {0:d}".format(NrOfBursts)).readlines():
  print i
  sleep(0.1)
  datafile.write(i)
  datafile.write('\n')
print "4"

# sleep(0.01)

for delay in range(0,64):            # loop over all possible pll delay settings
  
          
  inputo= str(bin(delay))+str(11)    # since 64 are just 6 bits, but we need 8 (where the last 2 are just space holder)
  inputo= int(inputo,2)              # we need to add by hand two '1' at the end of the binary string
          
  print "inputo " +str(inputo)       # print the new number (with the two '1' in the back) as a binary string
  pxfec.send("tbmplldelay "+ str(inputo)).readlines()     # send the number as a command to set the delay settings
 # sleep(1)

 # for l in range (1):
  #  digfed.send("test 100").readlines()


  bdelay=bin(delay)
  if len(bdelay)>5:
    x=bdelay[-3:]                    # last 3 elements -> give us value for 400MHz phase adjust (0-7)
    y=bdelay[:-3]                    # first 3 elements -> give us value for 160MHz phase adjust (0-7)

  if len(bdelay) <=5:                 # y is zero therefore we have only 5 bits (3 of 'x' and the two '1')
    x=bdelay
    y=bin(0)

    
  x = int(x, 2)
  y = int(y, 2)

  datafile.write("{0:d}".format(y))
#  sleep(0.1)
  datafile.write("\t")
#  sleep(0.1)
  datafile.write("{0:d}".format(x))
#  sleep(0.1)
  datafile.write("\t")
#  sleep(0.1)
  print "begin with testprogram for ",str(y), "and ", str(x)

  for l in digfed.send("test {0:d}".format(NrOfBursts)).readlines():
   # sleep(0.1)
    print l
    datafile.write(l)
    datafile.write('\n')

print "finished"

print "5"

'''
for l in digfed.send("test {0:d}".format(NrOfBursts)).readlines():
  print l
  datafile.write(l)
  datafile.write('\n')


print "finished"

ccu.close()
digfed.close()
pxfec.close()
'''
