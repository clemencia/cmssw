import os

list_timing_iovs=[]

list_realali_iovs=[]

ldir = os.listdir(".")
#print( ldir  )

for fname in ldir:
    if "timing_alignment_iov" in fname and ".xml" in fname:
        #print( fname )
        iov = int(fname.split("v")[-1].strip(".xml"))
        print(iov)
        list_timing_iovs.append(iov)
    elif "real_alignment_iov" in fname and ".xml" in fname:
        iov = int(fname.split("v")[-1].strip(".xml"))
        print(iov)
        list_realali_iovs.append(iov)

list_realali_iovs.sort()
list_timing_iovs.sort()

#check repeated successive iov payloads

import subprocess

list_goodreal_iov=[]
for i,iov in enumerate(list_realali_iovs):
    if i>0 :
        previov = list_realali_iovs[i-1]

        a= subprocess.check_output(
            "diff real_alignment_iov"+str(iov)+".xml real_alignment_iov"+str(previov)+".xml  | wc"
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
            "diff timing_alignment_iov"+str(iov)+".xml timing_alignment_iov"+str(previov)+".xml  | wc"
            ,shell=True)
        if int(a.split()[0] ) ==4 :
            print( a )
            print("skipping file of IOV ",iov)
            continue
    list_goodtiming_iov.append(iov)
    print("Timing Vertical Alignment for  IOV start ",iov)



a = [ item for item in list_goodtiming_iov if item not in list_realali_iovs]
b = [ item for item in list_goodtiming_iov if item not in list_goodreal_iov]

print("number of 'good' iovs for global alignment: ", len(list_goodreal_iov))
print("number of 'good' iovs for timing rp alignments : ",len(list_goodtiming_iov))

print("a (should be empty)",a)
print("b (iovs to add to rpix_timing_iovs in script run_rp..._v2.py):\n")
bs = [str(i) for i in b]
print(bs)
