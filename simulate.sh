  #!/bin/bash          
MIN_ENERGY=60
MAX_ENERGY=60
DELTA_ENERGY=1
MIN_BORON=230
MAX_BORON=249
DELTA_BORON=1
NUM_EVENTS=30000


ENERGY=$MIN_ENERGY
BORON=$MIN_BORON

until [  $ENERGY -gt $MAX_ENERGY ]; do
	BORON=$MIN_BORON
	until [  $BORON -gt $MAX_BORON ]; do		
		
		FILENAME="neutron_capture_pos_"$ENERGY"keV_"$BORON"pc.txt"
		echo $FILENAME writing
		#echo "Step#    X(mm)    Y(mm)    Z(mm) KinE(MeV)  dE(MeV) StepLeng TrackLeng  NextVolume ProcName">$FILENAME
		./exampleN01 --neutronEnergy $ENERGY --neutronEnergySpread 0 --numberOfEvents $NUM_EVENTS --edgeCells 3 --boronLoading $BORON --boronEnrichment 100|grep alpha -A4 |grep initStep >$FILENAME
		let BORON+=$DELTA_BORON	
	done

        let ENERGY+=$DELTA_ENERGY
done
