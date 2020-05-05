#!/bin/sh

echo "Splitting global alignment files"

python3 script_tables_global.py 

echo "Creating file for iov 309055"
cp real_alignment_iov307082.xml real_alignment_iov309055.xml
sed -i "s/307082/309055/g" real_alignment_iov309055.xml

echo "Creating file for iov 286693"
cp real_alignment_iov284025.xml real_alignment_iov286693.xml
sed -i "s/284025/286693/g" real_alignment_iov286693.xml
sed -i "s/2840/2867/g" real_alignment_iov286693.xml

mkdir globalfiles
mv real_alignment*xml globalfiles

echo "Splitting timing alignment files"
python3 script_tables_timing.py 

mkdir timingfiles
mv timing_alignment*xml timingfiles

echo "Checking IOV boundaries"
python3 check_iov_boundaries_glob_vs_time_v3.py


echo "Files with list of good IOVs to give to the SQLite writer"
ls list*txt

