import os,subprocess



ldir = os.listdir("globalfiles")
list_realali_iovs = [int(f.split("v")[-1].strip(".xml")) for f in ldir if "real_alignment_iov" in f and "~" not in f  ]

gstr = subprocess.getoutput("grep '<iov first'  F*timing*xml")
glist=gstr.split("\n")
#
list_timing_iovs=[]
for item in glist:
    print(item)
    iov=int(item.split("first=")[-1].split(":0")[0].lstrip('"'))
    list_timing_iovs.append(iov)

list_realali_iovs.sort()
list_timing_iovs.sort()


#check repeated successive iov payloads

import subprocess

list_goodreal_iov=[]
for i,iov in enumerate(list_realali_iovs):
    if i>0 :
        previov = list_realali_iovs[i-1]

        a= subprocess.check_output(
            "diff globalfiles/real_alignment_iov"+str(iov)+".xml globalfiles/real_alignment_iov"+str(previov)+".xml  | wc"
            ,shell=True)
        if int(a.split()[0] ) ==4 :
            print( a )
            print("skipping file of IOV ",iov)
            continue
    list_goodreal_iov.append(iov)
    print("Real Alignment for  IOV start ",iov)


list_goodtiming_iov=[]
for i,iov in enumerate(list_timing_iovs):
    if i>0 :
        previov = list_timing_iovs[i-1]

        a= subprocess.check_output(
            "diff timingfiles/timing_alignment_iov"+str(iov)+".xml timingfiles/timing_alignment_iov"+str(previov)+".xml  | wc"
            ,shell=True)
        if int(a.split()[0] ) ==4 :
            print( a )
            print("skipping timing file of IOV ",iov)
            continue
    list_goodtiming_iov.append(iov)
    print("Timing Vertical Alignment for  IOV start ",iov)



a = [ item for item in list_goodtiming_iov if item not in list_realali_iovs]
b = [ item for item in list_goodtiming_iov if item not in list_goodreal_iov]

print("number of 'good' iovs for global alignment: ", len(list_goodreal_iov))
print("number of 'good' iovs for timing rp alignments : ",len(list_goodtiming_iov))
gs = [str(i)+"\n" for i in list_goodreal_iov]
fglobal=open("list_realgood.txt","w")
fglobal.writelines(gs)
fglobal.close()
print("a (should be empty)",a)
print("b (iovs to add to rpix_timing_iovs in script run_rp..._v2.py):\n")
bs = [str(i)+"\n" for i in b]
print(bs)
ftime=open("list_timing.txt","w")
ftime.writelines(bs)
ftime.close()
