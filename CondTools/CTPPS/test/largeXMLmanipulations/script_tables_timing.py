from __future__ import print_function
import subprocess as sp

#import sys

#xmlname = sys.argv[1]


listxmls = sp.getoutput("ls F*timing*xml").split("\n")

#timingfiles = ["timing_RP_2017_preTS2_v2.xml","timing_RP_2017_postTS2_v3.xml","timing_RP_2018_v4.xml"]
#for tfile in timingfiles:
for xmlname in listxmls:
    infile   = open(xmlname,"rt")
    xmllines = infile.readlines()
    infile.close()
    #tree = ET.parse(infilename)
    #root = tree.getroot()
    outfile =open("test.xml","wt")
    iov = 0
    
    firstline = xmllines.pop(0)
    #secondline = xmllines.pop(0)
    lastline = xmllines.pop(-1)
    
    for line in xmllines:
        if "iov" not in line:
            outfile.write(line)
        else:
            if "</iov>" in line:
                outfile.write(line)
                outfile.write(lastline)
                outfile.close()
                outfilename = "timing_alignment_iov"+str(iov)+".xml"
                output = sp.run("mv test.xml "+outfilename, shell=True, check=True)
                outfile=open("test.xml","wt")
            else:
                outfile.write(firstline)
                # outfile.write(secondline)
                outfile.write(line)
                iovfirstfield = line.split()[1].split("=\"")[1].split(":")[0]
                iov = int(iovfirstfield)
                print (iov)
    if not  outfile.closed:
        outfile.close()
