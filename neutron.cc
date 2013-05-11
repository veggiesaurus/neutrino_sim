#include "G4RunManager.hh"
#include "G4VisManager.hh"
#include "G4VisExecutive.hh"
#include "G4UIExecutive.hh"
#include "G4UImanager.hh"
#include "QGSP_BERT_HP.hh"
#include "G4StepLimiterBuilder.hh"
#include "NeutronDetectorConstruction.hh"
#include "NeutronPrimaryGeneratorAction.hh"
#include "NeutronSimTrackingAction.hh"
#include "NeutronSimEventAction.hh"
#include <boost/program_options.hpp>
#include <boost/format.hpp>
namespace po = boost::program_options;
using boost::format;
#include <iostream>
#include <fstream>
#include <iterator>
#include <string>
using namespace std;

po::variables_map vm;

//program parameters
G4int numberOfEvents, vis;
string outputVerboseFile, outputStatisticsFile;
bool appendStats, appendVerbose;
bool outputRealistic;

//incident particles
G4double neutronEnergy, neutronEnergySpread; 
G4double incidentAngle;
//detector material
G4double boronLoading, boronEnrichment;
//detector geometry
G4double hexRadius, hexLength;
G4int edgeCells;
//detector performance
G4double verticalResolution;

bool parseVariables(int argc, char* argv[])
{
	try {

		po::options_description desc("Allowed options");
		desc.add_options()
			("help", "produce help message")
			("numberOfEvents", po::value<G4int>(&numberOfEvents)->default_value(25000), "set number of events")
			("neutronEnergy", po::value<G4double>(&neutronEnergy)->default_value(1000), "set incident neutron energy (in keV)")
			("neutronEnergySpread", po::value<G4double>(&neutronEnergySpread)->default_value(0), "set incident neutron energy spread (in keV)")
			("incidentAngle", po::value<G4double>(&incidentAngle)->default_value(0), "set incident neutron angle in the x-y plane (in degrees. 0: along y axis; 90: along +ve x axis)")
			("boronLoading", po::value<G4double>(&boronLoading)->default_value(5), "set percentage of boron in scintillator (0-100)")
			("boronEnrichment", po::value<G4double>(&boronEnrichment)->default_value(19.89f), "set percentage of B-10 enrichment of boron (0: All B-11; 100: All B-10)")
			("hexRadius", po::value<G4double>(&hexRadius)->default_value(25), "set radius of hexagonal scintillator cell (in mm)")
			("hexLength", po::value<G4double>(&hexLength)->default_value(300), "set length of hexagonal scintillator cell (in mm)")
			("edgeCells", po::value<G4int>(&edgeCells)->default_value(4), "set number of hexagonal cells along each edge ")
			("verticalResolution", po::value<G4double>(&verticalResolution)->default_value(21), "set sigma of vertical measurements (in mm)")
			("outputVerboseFile", po::value<string>(&outputVerboseFile)->default_value(""), "set output file for each neutron capture position output")
			("outputStatisticsFile", po::value<string>(&outputStatisticsFile)->default_value(""), "set output file for statistics output")
			("appendStats", "append statistics output instead of overwriting")			
			("appendVerbose", "append verbose output instead of overwriting")
			("ui", "load ui")
			("vis", po::value<G4int>(&vis)->default_value(0), "load vis")
			("outputRealistic", "output realistic reconstructions, taking into account finite resolution of detectors")
			;

		po::store(po::parse_command_line(argc, argv, desc), vm);
		po::notify(vm);    

		appendStats=vm.count("appendStats");		
		appendVerbose=vm.count("appendVerbose");		
		outputRealistic=vm.count("outputRealistic");		
		//vis=vm.count("ui")?1:0;

		if (vm.count("help")) {
			cout << desc << "\n";
			return 1;
		}

		cout << "number of events was set to " << numberOfEvents << ".\n";
		cout << "neutron energy was set to " << neutronEnergy << " +- "<<neutronEnergySpread<<" keV.\n";        
		cout << "boron loading was set to " << boronLoading << " %.\n";
		cout << "boron enrichment was set to " << boronEnrichment << " %.\n";
		cout << "hexagonal scintillator cell radius was set to " << hexRadius << " mm.\n";
		cout << "hexagonal scintillator cell length was set to " << hexLength << " mm.\n";
		cout << "number of hexagonal edge cells was set to " << edgeCells << ".\n";	
		if (!outputVerboseFile.empty())
			cout << "outputing verbose neutron capture positions to " << outputVerboseFile << ".\n";	
		if (!outputStatisticsFile.empty())
			cout << "outputing statistical info to " << outputStatisticsFile << ".\n";	


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

	ofstream* verboseOut=NULL;
	if (!outputVerboseFile.empty())   
	{
		verboseOut=new ofstream(outputVerboseFile.c_str(), ios::out | (appendVerbose?ios::app:ios::trunc));
		if (!appendVerbose)
			(*verboseOut)  << format("%9s  %9s  %9s  %9s") % "X" % "Y" % "Z" % "THETA"<<endl;			
	}

	// Construct the default run manager
	//
	G4RunManager* runManager = new G4RunManager;

	// set mandatory initialization classes
	//
	NeutronDetectorConstruction* detector = new NeutronDetectorConstruction(boronLoading/100.0, boronEnrichment, hexRadius, hexLength, edgeCells);
	//detector->SetMaxStep(1.0*mm);
	runManager->SetUserInitialization(detector);
	//
	G4VModularPhysicsList* physics = new QGSP_BERT_HP();
	physics->RegisterPhysics(new G4StepLimiterBuilder());
	runManager->SetUserInitialization(physics);

	// set mandatory user action class
	//
	G4VUserPrimaryGeneratorAction* gen_action = new NeutronPrimaryGeneratorAction(neutronEnergy, neutronEnergySpread, incidentAngle);
	runManager->SetUserAction(gen_action);
	NeutronSimEventAction* event_action=new NeutronSimEventAction();
	runManager->SetUserAction(event_action);
	NeutronSimTrackingAction* tracking_action=new NeutronSimTrackingAction(verboseOut, verticalResolution, outputRealistic);
	runManager->SetUserAction(tracking_action);

	// Initialize G4 kernel
	//
	//vis=0;
	runManager->Initialize();
	if (vis>1)
	{
		G4VisManager* visManager = new G4VisExecutive;
		visManager->Initialize();
	}
	G4UIExecutive * ui;
	if (vis)  
		ui = new G4UIExecutive(argc,argv);

	// Get the pointer to the UI manager and set verbosities
	//
	G4UImanager* UI = G4UImanager::GetUIpointer();



	UI->ApplyCommand("/run/verbose 0");
	UI->ApplyCommand("/event/verbose 0");
	UI->ApplyCommand("/tracking/verbose 1");
	if (vis>1)
		UI->ApplyCommand("/control/execute vis.mac"); 
	//UI->ApplyCommand("/Neutron/det/stepMax 1.0 mm"); 
	// Start a run  
	runManager->BeamOn(numberOfEvents);

	G4int numTracks;
	G4double alphaMeanX, alphaSigmaX, alphaMeanY, alphaSigmaY, alphaMeanZ, alphaSigmaZ, recMeanTheta, recSigmaTheta, alphaMeanT, alphaSigmaT;
	tracking_action->GetStatistics(numTracks, alphaMeanX, alphaSigmaX, alphaMeanY, alphaSigmaY, alphaMeanZ, alphaSigmaZ, recMeanTheta, recSigmaTheta, alphaMeanT, alphaSigmaT);
	G4cout <<"======================================================================"<<G4endl;
	G4cout<<"Statistical output"<<endl;
	G4cout <<"======================================================================"<<G4endl;
	G4cout  << format("%4s  %5s  %9s  %9s  %9s  %9s  %9s  %9s  %9s  %9s %9s %9s") % "E" % "boron" % "mu_x" % "s_x" % "mu_y" % "s_y" % "mu_z" % "s_z" % "mu_theta" % "s_theta" % "mu_t" % "s_t"<<endl;					
	G4cout  << format("%4.1f  %5.1f  %9.4f  %9.4f  %9.4f  %9.4f  %9.4f  %9.4f  %9.4f  %9.4f %9.4f  %9.4f") %neutronEnergy%boronLoading%alphaMeanX%alphaSigmaX%alphaMeanY%alphaSigmaY%alphaMeanZ%alphaSigmaZ%recMeanTheta%recSigmaTheta%alphaMeanT%alphaSigmaT<<endl;			

	ofstream* statsOut=NULL;
	if (!outputStatisticsFile.empty())
	{
		statsOut=new ofstream(outputStatisticsFile.c_str(), ios::out | (appendStats?ios::app:ios::trunc));   
		if (!appendStats)
			(*statsOut)  << format("%4s  %5s  %9s  %9s  %9s  %9s  %9s  %9s  %9s  %9s %9s %9s") % "E" % "boron" % "mu_x" % "s_x" % "mu_y" % "s_y" % "mu_z" % "s_z" % "mu_theta" % "s_theta" % "mu_t" % "s_t"<<endl;					
		(*statsOut)  << format("%4.1f  %5.1f  %9.4f  %9.4f  %9.4f  %9.4f  %9.4f  %9.4f  %9.4f  %9.4f %9.4f  %9.4f") %neutronEnergy%boronLoading%alphaMeanX%alphaSigmaX%alphaMeanY%alphaSigmaY%alphaMeanZ%alphaSigmaZ%recMeanTheta%recSigmaTheta%alphaMeanT%alphaSigmaT<<endl;			
		statsOut->close();
	} 
	if (verboseOut)
		verboseOut->close();
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


