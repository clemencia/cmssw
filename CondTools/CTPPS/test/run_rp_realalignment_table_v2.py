#! python3

from __future__ import print_function

### this script takes a file with a list of IOVs and subdir name where separated xmls files are located
### the xml files have the iov start runno in the name and in the body (iov node of the xml) which should match
### for large xml with multiple IOVs we preferred to break into single-iov xmls to check for identical consecutive payloads
### for each new IOV start cmsRun write... is called to update the sqlite file
### the resulting sqlite file has a table of several IOV start and payloads

import subprocess
import sys

iovs_file = open(sys.argv[1])
path=sys.argv[2]
dryRun = bool(sys.argv[3]=="true" or sys.argv[3]=="True" or sys.argv[3] == "TRUE" or sys.argv[3]=="1")
print (dryRun)
if len(sys.argv) > 5:
    tagname = sys.argv[4]
    filename = sys.argv[5]
else:
    tagname=""
    filename=""

iovs_lines = iovs_file.readlines()
iovs_list = [(x.split("_iov")[1]).split(".xml")[0] for x in iovs_lines]
iovs_list.sort()
iovs_file.close()

rpix_timing_iovs = ["286693","309055", # RPix IOV start 2017 e 2018
                    #                ### old    "294730","303615","314158" # Timing 2017 preTS2, 2017 postTS2 and 2018
                    "303998", "306029", "322179"   
                    #  boundaries for timing alignments ### b=[ item for item in list_goodtiming_iov if item not in list_goodreal_iov]
                    ]

for i in range(0,len(iovs_list)):
    runno = iovs_list[i].strip()
    if i >0 and runno not in rpix_timing_iovs: ## runs where rpix or timing change
        previov = iovs_list[i-1].strip()
        a= subprocess.check_output(
            "diff "+path+"/real_alignment_iov"+runno+".xml "+path+"/real_alignment_iov"+previov+".xml  | wc"
            ,shell=True)
        if int(a.split()[0] ) ==4 :
            #print( a )
            print("skipping file of IOV "+runno)
            continue
    if dryRun:
        continue
    print("Processing payload for IOV start "+runno)
    print (subprocess.check_output(
        "cmsRun write-ctpps-rprealalignment_table_cfg_v2.py "+runno+"  "+path +" "+tagname+" "+filename
        ,shell=True )
        )

print("finished")
