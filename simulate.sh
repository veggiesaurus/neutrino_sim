 #!/bin/bash 

#ranges         
MIN_ENERGY=60
MAX_ENERGY=60
DELTA_ENERGY=1
MIN_BORON=1
MAX_BORON=99
DELTA_BORON=1

#parameters
ENERGY_SPREAD=0
BORON_ENRICHMENT=100
NUM_EVENTS=100
STATS_OUTPUT=statsOut.txt
EDGE_CELLS=3

#for debugging
#SIM_EXE="echo ./neutrino"
SIM_EXE="./neutrino"



ENERGY=$MIN_ENERGY
BORON=$MIN_BORON
#don't append the first event
$SIM_EXE --outputStatisticsFile  $STATS_OUTPUT --neutronEnergy $ENERGY --neutronEnergySpread $ENERGY_SPREAD --numberOfEvents $NUM_EVENTS --edgeCells $EDGE_CELLS --boronLoading $BORON --boronEnrichment $BORON_ENRICHMENT 

until [  $ENERGY -gt $MAX_ENERGY ]; do
	BORON=$MIN_BORON
	until [  $BORON -gt $MAX_BORON ]; do						
		$SIM_EXE --appendStats --outputStatisticsFile  $STATS_OUTPUT --neutronEnergy $ENERGY --neutronEnergySpread $ENERGY_SPREAD --numberOfEvents $NUM_EVENTS --edgeCells $EDGE_CELLS --boronLoading $BORON --boronEnrichment $BORON_ENRICHMENT 
		let BORON+=$DELTA_BORON	
	done
        let ENERGY+=$DELTA_ENERGY
done
