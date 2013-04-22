#include "G4RunManager.hh"
#include "G4VisManager.hh"
#include "G4VisExecutive.hh"
#include "G4UIExecutive.hh"
#include "G4UImanager.hh"
#include "QGSP_BERT_HP.hh"
#include "ExN01DetectorConstruction.hh"
#include "ExN01PrimaryGeneratorAction.hh"

#include <boost/program_options.hpp>
namespace po = boost::program_options;

#include <iostream>
#include <iterator>
using namespace std;

po::variables_map vm;

//program parameters
G4int numberOfEvents, vis;
//incident energy
G4double neutronEnergy, neutronEnergySpread; 
G4double positronEnergy, positronEnergySpread;

//detector material
G4double boronLoading, boronEnrichment;
//detector geometry
G4double hexRadius, hexLength;
G4int edgeCells;

bool parseVariables(int argc, char* argv[])
{
	try {

        po::options_description desc("Allowed options");
        desc.add_options()
            ("help", "produce help message")
            ("numberOfEvents", po::value<G4int>(&numberOfEvents)->default_value(25000), "set number of events")
            ("neutronEnergy", po::value<G4double>(&neutronEnergy)->default_value(60), "set incident neutron energy (in keV)")
            ("neutronEnergySpread", po::value<G4double>(&neutronEnergySpread)->default_value(0), "set incident neutron energy spread (in keV)")
            ("positronEnergy", po::value<G4double>(&positronEnergy)->default_value(10), "set incident positron energy (in keV)")
            ("positronEnergySpread", po::value<G4double>(&positronEnergySpread)->default_value(0), "set incident positron energy spread (in keV)")
            ("boronLoading", po::value<G4double>(&boronLoading)->default_value(5), "set percentage of boron in scintillator (0-100)")
            ("boronEnrichment", po::value<G4double>(&boronEnrichment)->default_value(19.9), "set percentage of B-10 enrichment of boron (0: All B-11; 100: All B-10)")
            ("hexRadius", po::value<G4double>(&hexRadius)->default_value(25), "set radius of hexagonal scintillator cell (in mm)")
            ("hexLength", po::value<G4double>(&hexLength)->default_value(300), "set length of hexagonal scintillator cell (in mm)")
            ("edgeCells", po::value<G4int>(&edgeCells)->default_value(3), "set number of hexagonal cells along each edge ")
            ("vis", po::value<G4int>(&vis)->default_value(0), "0: no vis; 1: neutrons only; 2: full")
            
        ;
       
        po::store(po::parse_command_line(argc, argv, desc), vm);
        po::notify(vm);    

        if (vm.count("help")) {
            cout << desc << "\n";
            return 1;
        }
        
        cout << "number of events was set to " << numberOfEvents << ".\n";
        cout << "neutron energy was set to " << neutronEnergy << " +- "<<neutronEnergySpread<<" keV.\n";        
        cout << "positron energy was set to " << positronEnergy << " +- "<<positronEnergySpread<<" keV.\n";
        cout << "boron loading was set to " << boronLoading << " %.\n";
        cout << "boron enrichment was set to " << boronEnrichment << " %.\n";
        cout << "hexagonal scintillator cell radius was set to " << hexRadius << " mm.\n";
        cout << "hexagonal scintillator cell length was set to " << hexLength << " mm.\n";
        cout << "number of hexagonal edge cells was set to " << edgeCells << ".\n";	
        
    }
    catch(exception& e) {
        cerr << "error: " << e.what() << "\n";
        return 1;
    }
    catch(...) {
        cerr << "Exception of unknown type!\n";
    }
    return 0;
    
}




int main(int argc,char** argv)
{
   if (parseVariables(argc, argv))
	return 1;

  // Construct the default run manager
  //
  G4RunManager* runManager = new G4RunManager;

  // set mandatory initialization classes
  //
  G4VUserDetectorConstruction* detector = new ExN01DetectorConstruction(boronLoading/100.0, boronEnrichment, hexRadius, hexLength, edgeCells);
  runManager->SetUserInitialization(detector);
  //
  G4VUserPhysicsList* physics = new QGSP_BERT_HP();
  runManager->SetUserInitialization(physics);

  // set mandatory user action class
  //
  G4VUserPrimaryGeneratorAction* gen_action = new ExN01PrimaryGeneratorAction(neutronEnergy, neutronEnergySpread, positronEnergy, positronEnergySpread);
  runManager->SetUserAction(gen_action);

  // Initialize G4 kernel
  //
  runManager->Initialize();
  G4VisManager* visManager = new G4VisExecutive;
  visManager->Initialize();
  G4UIExecutive * ui;
  if (vis)  
    G4UIExecutive * ui = new G4UIExecutive(argc,argv);

  // Get the pointer to the UI manager and set verbosities
  //
  G4UImanager* UI = G4UImanager::GetUIpointer();

    
  
  UI->ApplyCommand("/run/verbose 1");
  UI->ApplyCommand("/event/verbose 1");
  UI->ApplyCommand("/tracking/verbose 1");
  if (vis)
     UI->ApplyCommand("/control/execute vis.mac");   
  runManager->BeamOn(numberOfEvents);

  // Start a run
  //
  if (vis)
  {
     ui->SessionStart();
     delete ui;
  }
  // Job termination
  //
  // Free the store: user actions, physics_list and detector_description are
  //                 owned and deleted by the run manager, so they should not
  //                 be deleted in the main() program !
  //
  delete runManager;

  return 0;
}


