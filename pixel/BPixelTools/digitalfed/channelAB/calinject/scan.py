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
pxfec.send("roc 0:15").readlines()
pxfec.send("trim 0").readlines()
pxfec.send("mask").readlines()


for roc in range(0, 16, 1):
  pxfec.send("mask").readlines()
  pxfec.send("roc {0:d}".format(roc)).readlines()
  pxfec.send("unmask").readlines()

  for caldel in range(40, 200, 20):
    pxfec.send("CalDel {0:d}".format(caldel)).readlines()
    print "roc: ", roc, "\tcaldel: ", caldel, "\n"

    for i in digfed.send("testcal 100").readlines():
      print i


print "finished"



ccu.close()
digfed.close()
pxfec.close()
