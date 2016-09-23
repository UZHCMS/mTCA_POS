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


ts = time.time()
st = datetime.datetime.fromtimestamp(ts).strftime('Frequency_%d-%m-%Y_%H:%M:%S.txt')


datafile = open(st, 'w')     # where 'datafile' is the name of the created text file (take care, this will overwrite the existing file!!)

for i in digfed.send("title {0:d}".format(100)).readlines():
  print i
  datafile.write(i)
  datafile.write('\n')

for hertz in range (1, 1002, 10):
  rate = int(1/(hertz *25*10**(-6)))
  digfed.send("rate {0:d}".format(rate)).readlines()
  digfed.send("confamc").readlines()
  for l in digfed.send("test 100").readlines():
    print l
    datafile.write(l)
    datafile.write('\n')
