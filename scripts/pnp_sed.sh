#!/bin/bash
sed -e 's/``/"/g' -e "s/''/\"/g" < data/pnp_austen.txt > data/pnp_austen_cs296.txt

#Cast:
#Fitzwilliam - Abhinav
#Darcy - Anant
#Catherine - Stephan

sed -e 's/Fitzwilliam/Abhinav/g;s/Darcy/Anant/g;s/Catherine/Stephan/g' < data/pnp_austen_cs296.txt > data/pnp_austen_temp.txt
mv data/pnp_austen_temp.txt data/pnp_austen_cs296.txt
