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
st = datetime.datetime.fromtimestamp(ts).strftime('calinject_%d-%m-%Y_%H:%M:%S.txt')




#variables to play with
NrOfBursts = 1
hertz = 40                   #in [kHz]
TriggerPerBurst = 1
gain = 2
bias = 30
tbmplldelay = 0
tbmadelay = 0

rate = int(1./(hertz *25*10**(-6)))
if (rate>65535):
  rate = 65536
  print "Warning: Rate set too high, set it on 65536 "

print rate
digfed.send("rate {0:d}".format(rate)).readlines()
digfed.send("burst {0:d}".format(TriggerPerBurst)).readlines()
digfed.send("mode 0").readlines()
digfed.send("confamc").readlines()



#create a textfile
datafile = open(st, 'w')     # where 'datafile' is the name of the created text file (take care, this will
                             # overwrite the existing file!!)




#initialize poh on gain=0 and bias=0
ccu.send("channel 11").readlines()
ccu.send("poh2 set g0 {0:d}".format(gain)).readlines()
ccu.send("poh2 set b0 {0:d}".format(bias)).readlines()


#initialize module (tbm, pll,...) with help of pxfec
pxfec.send("trim 7").readlines()
pxfec.send("mask").readlines()
pxfec.send("arm 20 20").readlines()
pxfec.send("tbm enable triggers").readlines()
pxfec.send("module 31").readlines()
pxfec.send("tbm").readlines()
pxfec.send("tbmplldelay {0:d}".format(tbmplldelay)).readlines()   # set the pll delay setting
pxfec.send("tbmadelay {0:d}".format(tbmadelay)).readlines()     #  set the roc delay settings


#setting title with function in digitalfed.cpp
datafile.write("#the gain was set to: " )
datafile.write("{0:d}".format(gain))
datafile.write('\t')
datafile.write("the bias was set to: ")
datafile.write("{0:d}".format(bias))
datafile.write('\t')
datafile.write("the tbmplldelay was set to: ")
datafile.write("{0:d}".format(tbmplldelay))
datafile.write('\t')
datafile.write("the tbmadelay was set to: ")
datafile.write("{0:d}".format(tbmadelay))
datafile.write('\n')
for i in digfed.send("titleroc {0:d}".format(NrOfBursts)).readlines():
  print i
  datafile.write(i)
  datafile.write('\n')


for i in range(0,1):
            
  inputo= str(bin(delay))            
  inputo= int(inputo,2)             
          
  print "inputo " +str(inputo)       # print the new number (with the two '1' in the back) as a binary string
  pxfec.send("tbmadelay "+ str(inputo)).readlines()     # send the number as a command to set the delay settings



 # for i in range(0, 100):
 #   digfed.send("testab 100").readlines()

  bdelay=bin(delay)
       
  b7 = ((delay&128)>>7)                   # b7: delay Head/Trail by 6.25ns
  b6 = (delay&64)>>6                      # b6: delay token Ins by 6.25ns
  d0 = delay&7                            # last 3 elements -> give us value for ROC delay port 0 (0-7)
  d1 = (delay&56)>>3                      # give us value for ROC delay port 1 (0-7)


  print "begin with testprogram for b6: ", b6, "\tb7: ", b7, "\tROC delay port_0: ", d0, "\tROC delay port_1: ", d1

  for l in digfed.send("testroc {0:d}".format(NrOfBursts)).readlines():
    print l
    datafile.write("{0:d}".format(b7))
    datafile.write("\t")
    datafile.write("{0:d}".format(b6))
    datafile.write("\t")
    datafile.write("{0:d}".format(d1))
    datafile.write("\t")
    datafile.write("{0:d}".format(d0))
    datafile.write("\t")
    datafile.write(l)
    datafile.write('\n')

print "finished"



ccu.close()
digfed.close()
pxfec.close()
