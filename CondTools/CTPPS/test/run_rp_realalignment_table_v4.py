#! python3

from __future__ import print_function

### this script takes a file with a list of IOVs and subdir name where separated xmls files are located
### the xml files have the iov start runno in the name and in the body (iov node of the xml) which should match
### for large xml with multiple IOVs we preferred to break into single-iov xmls to check for identical consecutive payloads
### for each new IOV start cmsRun write... is called to update the sqlite file
### the resulting sqlite file has a table of several IOV start and payloads

import subprocess
import sys


## to run do
## python3 run_rp_realalignment_table_v3.py largeXMLmanipulations <true or false> <tagname> <sqlitefilename>
##
 
path=sys.argv[1]
dryRun = bool(sys.argv[2].lower()=="true" or sys.argv[2].lower()=="yes"  or sys.argv[2]=="1" )
print (dryRun)
if len(sys.argv) > 4:
    tagname = sys.argv[3]
    filename = sys.argv[4]
else:
    tagname="PPSRPAlignment_test"
    filename="PPSRPAlignment.db"

gfile = open(path+"/list_realgood.txt","r")
iovs_list = gfile.readlines()
gfile.close()

tfile =open(path+"/list_timing.txt","r")
tiovs_list=tfile.readlines()
tfile.close()


rpix_iovs = ["286693","309055"#, # RPix IOV start 2017 e 2018
                    ]
### old    ["294730","303615","314158"] # Timing 2017 preTS2, 2017 postTS2 and 2018
tiovs_list = [ i.rstrip("\n") for i in tiovs_list]
iovs_list  = [ i.rstrip("\n") for i in iovs_list]
 
rpix_timing_iovs = rpix_iovs + tiovs_list

iovs_list = iovs_list+ rpix_timing_iovs

iovs_list.sort()
 

for i in range(0,len(iovs_list)):
    runno = iovs_list[i].strip()
    if i >0 and runno not in rpix_timing_iovs: ## runs where rpix or timing change
        previov = iovs_list[i-1].strip()
        a= subprocess.check_output(
            "diff "+path+"/globalfiles/real_alignment_iov"+runno+".xml "+path+"/globalfiles/real_alignment_iov"+previov+".xml  | wc"
            ,shell=True)
        if int(a.split()[0] ) ==4 :
            #print( a )
            print("skipping file of IOV "+runno)
            continue
    if dryRun:
        print("IOV start "+runno)
        continue
    print("Processing payload for IOV start "+runno)
    print (subprocess.check_output(
        "cmsRun write-ctpps-rprealalignment_table_cfg_v4.py "+runno+"  "+path +" "+tagname+" "+filename
        ,shell=True )
        )

print("finished")
