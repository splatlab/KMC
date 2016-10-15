#!/bin/sh

######################################################

echo "f_vesca 16 threads"
/usr/bin/time ./kmc -k28 -ci0 -cs100000000 -cx1000000000 -r -t16 @../../data/f_vesca.name.txt NA_f_vesca.res data
sleep 2
echo "f_vesca 12 threads"
/usr/bin/time ./kmc -k28 -ci0 -cs100000000 -cx1000000000 -r -t12 @../../data/f_vesca.name.txt NA_f_vesca.res data
/usr/bin/time ./jellyfish count -m 28 -C -s 650M -t 12 -o jelly2 -g f_vesca.gen -G 11 --timing=jelly2time_f_vesca_12
sleep 2
echo "f_vesca 8 threads"
/usr/bin/time ./kmc -k28 -ci0 -cs100000000 -cx1000000000 -r -t8 @../../data/f_vesca.name.txt NA_f_vesca.res data
/usr/bin/time ./jellyfish count -m 28 -C -s 650M -t 8 -o jelly2 -g f_vesca.gen -G 11 --timing=jelly2time_f_vesca_8
sleep 2
echo "f_vesca 4 threads"
/usr/bin/time ./kmc -k28 -ci0 -cs100000000 -cx1000000000 -r -t4 @../../data/f_vesca.name.txt NA_f_vesca.res data
sleep 2
#echo "f_vesca 2 threads"
#/usr/bin/time ./jellyfish count -m 28 -C -s 650M -t 2 -o jelly2 -g f_vesca.gen -G 11 --timing=jelly2time_f_vesca_2
#sleep 2

######################################################

echo "g_gallus 16 threads"
/usr/bin/time ./kmc -k28 -ci0 -cs100000000 -cx1000000000 -r -t16 @../../data/g_gallus.name.txt NA_g_gallus.res data
sleep 2
echo "g_gallus 12 threads"
/usr/bin/time ./kmc -k28 -ci0 -cs100000000 -cx1000000000 -r -t12 @../../data/g_gallus.name.txt NA_g_gallus.res data
sleep 2
echo "g_gallus 8 threads"
/usr/bin/time ./kmc -k28 -ci0 -cs100000000 -cx1000000000 -r -t8 @../../data/g_gallus.name.txt NA_g_gallus.res data
sleep 2
echo "g_gallus 4 threads"
/usr/bin/time ./kmc -k28 -ci0 -cs100000000 -cx1000000000 -r -t4 @../../data/g_gallus.name.txt NA_g_gallus.res data
sleep 2


######################################################

echo "m_balbisiana 16 threads"
/usr/bin/time ./kmc -k28 -ci0 -cs100000000 -cx1000000000 -r -t16 @../../data/m_balbisiana.name.txt NA_m_balbisiana.res data
sleep 2
echo "m_balbisiana 12 threads"
/usr/bin/time ./kmc -k28 -ci0 -cs100000000 -cx1000000000 -r -t12 @../../data/m_balbisiana.name.txt NA_m_balbisiana.res data
sleep 2
echo "m_balbisiana 8 threads"
/usr/bin/time ./kmc -k28 -ci0 -cs100000000 -cx1000000000 -r -t8 @../../data/m_balbisiana.name.txt NA_m_balbisiana.res data
sleep 2
echo "m_balbisiana 4 threads"
/usr/bin/time ./kmc -k28 -ci0 -cs100000000 -cx1000000000 -r -t4 @../../data/m_balbisiana.name.txt NA_m_balbisiana.res data
sleep 2

######################################################

echo "h_sapiens_1 16 threads"
/usr/bin/time ./kmc -k28 -ci0 -cs100000000 -cx1000000000 -r -t16 @../../data/h_sapiens_1.name.txt NA_h_sapiens_1.res data
sleep 2
echo "h_sapiens_1 12 threads"
/usr/bin/time ./kmc -k28 -ci0 -cs100000000 -cx1000000000 -r -t12 @../../data/h_sapiens_1.name.txt NA_h_sapiens_1.res data
sleep 2
echo "h_sapiens_1 8 threads"
/usr/bin/time ./kmc -k28 -ci0 -cs100000000 -cx1000000000 -r -t8 @../../data/h_sapiens_1.name.txt NA_h_sapiens_1.res data
sleep 2
echo "h_sapiens_1 4 threads"
/usr/bin/time ./kmc -k28 -ci0 -cs100000000 -cx1000000000 -r -t4 @../../data/h_sapiens_1.name.txt NA_h_sapiens_1.res data
sleep 2

######################################################

echo "h_sapiens_2 16 threads"
/usr/bin/time ./kmc -k28 -ci0 -cs100000000 -cx1000000000 -r -t16 @../../data/h_sapiens_2.name.txt NA_h_sapiens_2.res data
sleep 2
echo "h_sapiens_2 12 threads"
/usr/bin/time ./kmc -k28 -ci0 -cs100000000 -cx1000000000 -r -t12 @../../data/h_sapiens_2.name.txt NA_h_sapiens_2.res data
sleep 2
echo "h_sapiens_2 8 threads"
/usr/bin/time ./kmc -k28 -ci0 -cs100000000 -cx1000000000 -r -t8 @../../data/h_sapiens_2.name.txt NA_h_sapiens_2.res data
sleep 2
echo "h_sapiens_2 4 threads"
/usr/bin/time ./kmc -k28 -ci0 -cs100000000 -cx1000000000 -r -t4 @../../data/h_sapiens_2.name.txt NA_h_sapiens_2.res data
sleep 2
