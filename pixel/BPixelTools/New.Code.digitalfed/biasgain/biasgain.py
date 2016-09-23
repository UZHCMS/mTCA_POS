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
#print "connecting to pxfec,",
#pxfec=SimpleSocket( 'localhost', 2015)
#print " done"

print "connecting to ccu,",
ccu=SimpleSocket( 'localhost', 2005)
print " done"

print "connecting to digitalfed,",
digfed=SimpleSocket( 'localhost', 2060)
print " done"

#import actual time
ts = time.time()
st = datetime.datetime.fromtimestamp(ts).strftime('biasgain_%d-%m-%Y_%H:%M:%S.txt')

#variables to play with
bursts = 500
hertz = 100                   #in [kHz]

rate = int(1./(hertz *25*10**(-6)))
if (rate>65535):
  rate = 65536
  print "Warning: Rate set too high, set it on 65536 "
print rate
digfed.send("rate {0:d}".format(rate)).readlines()
digfed.send("confamc").readlines()


#initialize poh on gain=0 and bias=0
ccu.send("channel 11").readlines()
ccu.send("poh2 set g0 0").readlines()
ccu.send("poh2 set b0 40").readlines()

#create a textfile
datafile = open(st, 'w')     # where 'datafile' is the name of the created text file (take care, this will overwrite the existing file!!)

for i in digfed.send("titlegb {0:d}".format(bursts)).readlines():
  print i
  datafile.write(i)
  datafile.write('\n')

for gain in range (3, -1, -1):        #0, 1, 2, 3
  ccu.send("poh2 set g0 {0:d}".format(gain)).readlines()
  sleep(0.1)
  for bias in range (35, 8, -1):    #0, 1, 2, ... , 126, 127
    ccu.send("poh2 set b0 {0:d}".format(bias)).readlines()
    sleep(0.1)
    print "gain: ", gain, "\tbias: ", bias
    datafile.write("{0:d}".format(gain))
    datafile.write("\t")
    datafile.write("{0:d}".format(bias))
    datafile.write("\t")
    print "begin with testprogram"
    for l in digfed.send("test {0:d}".format(bursts)).readlines():
      print l
      datafile.write(l)
      datafile.write('\n')
  digfed.send("fiforeset").readlines()

print "finished"


'''
ccu.close()
digfed.close()
pxfec.close()
'''
