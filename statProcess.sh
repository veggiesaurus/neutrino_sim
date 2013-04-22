  #!/bin/bash          
MIN_ENERGY=60
MAX_ENERGY=60
DELTA_ENERGY=1
MIN_BORON=1
MAX_BORON=1000
DELTA_BORON=1
NUM_EVENTS=10000


ENERGY=$MIN_ENERGY
BORON=$MIN_BORON
echo "E Boron mu_x s_x mu_y s_y mu_z s_z mu_theta s_theta"
until [  $ENERGY -gt $MAX_ENERGY ]; do
	BORON=$MIN_BORON
	until [  $BORON -gt $MAX_BORON ]; do		
		
		FILENAME="neutron_capture_pos_"$ENERGY"keV_"$BORON"pc.txt"
		./stats $FILENAME
		let BORON+=$DELTA_BORON	
	done

        let ENERGY+=$DELTA_ENERGY
done
