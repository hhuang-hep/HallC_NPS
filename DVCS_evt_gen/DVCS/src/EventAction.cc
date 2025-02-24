//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
/// \file analysis/shared/src/EventAction.cc
/// \brief Implementation of the EventAction class
//
//
// $Id: EventAction.cc 92322 2015-08-27 14:54:05Z gcosmo $
//
// 

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "EventAction.hh"

#include "RunAction.hh"

#include "HistoManager.hh"

#include "G4RunManager.hh"
#include "G4Event.hh"
#include "G4Run.hh"

#include "B5HadCalorimeterHit.hh"
#include "G4HCofThisEvent.hh"
#include "G4VHitsCollection.hh"
#include "G4SDManager.hh"
#include "G4ios.hh"

#include "CrystalCoverHit.hh"
#include "CrystalFrontCoverHit.hh"
#include "PMTcoverHit.hh"

#include "FluxHit.hh"

#include "dvcsGlobals.hh"

#include "TProcessID.h"

#include "RFunction.C"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

time_t t_int = time(0); // Record the initial time

// Random values for mis-calibration
// Double_t misCoeff[1080] = {
//   1.12553,1.15399,1.3262,1.95054,0.83299,1.83929,0.614462,1.81014,0.515561,1.65698,0.770405,0.731244,1.66655,1.27092,1.77323,0.834937,0.941998,1.47556,0.6463,1.3822,0.573087,0.812691,1.27595,1.94003,1.80519,0.961902,1.13858,1.59352,1.79564,1.46622,0.929081,1.78833,0.872765,0.557843,1.18708,1.59276,1.91674,1.07716,1.32033,1.11153,0.876385,1.06181,0.672582,1.75226,1.98352,1.67575,0.670233,0.526235,0.951447,1.2419,1.5136,1.73467,1.76999,1.13027,0.639662,1.97629,0.631024,1.04766,1.88605,0.95131,1.7367,0.550632,1.33091,1.06865,0.827968,0.731431,1.31878,0.889057,0.944374,1.89122,0.778363,0.660107,1.46403,0.803303,1.35341,0.966245,1.87866,0.572272,1.251,1.28287,1.03313,0.912839,0.885564,0.569061,1.43056,0.80391,1.4844,1.47133,1.24954,0.729581,0.801505,1.82882,1.40953,1.57908,0.843205,0.82203,1.75542,1.5984,1.50842,1.31511,1.2746,1.39652,1.14817,0.721881,0.62561,0.512847,0.744057,0.850417,1.41719,0.674088,1.41826,1.06259,1.77798,0.727448,0.794138,1.06275,1.17712,1.91398,1.77177,1.51693,0.666996,0.735488,1.5447,0.659097,1.26009,0.657758,1.28575,1.79996,1.3558,0.712445,1.47523,1.67136,1.12926,1.71711,1.49199,0.729824,1.91578,1.62401,1.23644,1.62186,1.87608,1.85309,1.60663,0.640016,0.924563,1.24716,1.05004,1.36311,1.82196,1.86288,1.65469,1.92309,0.72155,1.26159,1.47849,0.970429,1.6961,0.644404,1.68327,1.75009,1.91101,0.9815,1.53345,1.03547,1.73933,0.946609,1.84757,1.3967,1.50439,0.578968,1.48662,0.724103,0.639886,1.19006,1.70313,1.03616,1.25425,1.28689,1.92568,1.34408,1.28603,1.81485,1.73503,0.602684,0.69583,1.31405,1.13006,1.26797,1.13406,1.61769,1.31241,1.63595,1.10298,1.95155,1.83099,1.53426,1.80186,1.4147,1.48219,1.92145,0.670857,0.836309,1.77185,1.89583,1.94039,1.14099,1.25445,0.66636,1.76493,0.649886,1.67205,0.849352,0.82764,0.572366,0.730882,0.553384,0.655493,1.00138,0.52024,1.04023,1.31986,1.28904,1.72308,1.22215,0.938249,1.84244,0.691207,1.13572,0.645537,1.4465,1.67224,0.990644,0.724204,0.546451,1.89588,0.616799,1.98171,0.598008,0.964747,0.94026,1.42242,1.52703,0.811453,0.936016,1.86028,1.21947,1.04927,1.53798,1.2754,0.822169,1.45642,0.50812,0.89309,1.26809,1.1959,0.568676,1.89474,0.626941,1.53533,0.644918,1.7014,1.08823,1.35993,1.6647,1.28288,0.619496,0.764946,0.723468,0.887477,1.54119,1.45833,0.871581,0.609093,0.923743,1.15598,1.94567,1.22676,0.793734,1.37868,0.699884,1.18166,1.05264,1.39612,1.76882,0.878603,0.717536,0.716728,1.3299,1.5907,0.910119,0.647479,0.876943,1.72361,1.50204,1.43323,0.957007,1.92439,1.53155,0.949332,1.17668,1.00592,1.45442,1.62522,1.1251,0.73275,0.935127,0.980946,1.67669,1.68193,1.72778,1.18181,0.82509,0.748309,1.87531,1.42502,1.74586,0.956525,1.38009,1.99767,1.57153,1.82892,1.11886,1.96008,0.590475,1.8916,1.27928,1.17194,0.863894,1.36074,1.24367,1.92001,1.87726,1.31494,0.857205,1.64077,1.1606,1.69986,0.676303,1.45662,0.92213,0.89775,0.597552,0.612584,0.970829,1.05639,1.39043,0.899693,0.909287,0.821074,1.1482,0.516899,1.8094,1.15427,1.59722,0.846844,1.23045,1.5849,1.57042,1.81842,1.24667,0.665298,0.523927,0.658111,1.13785,1.91218,0.657264,0.636069,1.12274,1.475,1.04869,1.50614,0.844781,1.5415,0.703428,0.804763,1.10626,0.677726,1.39149,1.70606,0.807415,1.37795,1.63097,1.15987,1.6784,0.624537,1.6073,1.38797,1.00256,0.93682,1.84588,0.587834,1.04755,0.799138,1.26402,1.22888,0.962114,0.818481,1.61201,1.06259,1.50309,1.5938,0.576286,0.77356,1.50324,1.55238,1.87732,1.77856,1.24047,0.816857,1.8311,1.76804,1.91745,1.48518,0.543099,0.594476,1.06641,0.528467,0.980787,0.935254,0.973469,1.52417,0.870887,1.00571,1.72439,1.63708,1.8076,1.93109,1.72764,1.04469,0.733902,1.88685,1.85549,1.21724,1.03024,1.89982,0.855613,1.30731,0.748417,1.38479,0.757788,1.95671,1.09201,1.5516,1.75904,1.46489,1.64962,0.970325,1.36483,1.33795,1.67063,1.65948,1.82964,1.85002,1.50305,1.11835,0.873134,0.772861,1.70895,1.06448,1.81461,1.02449,1.19455,1.7132,1.5625,1.79878,1.13032,1.62931,0.656556,1.53673,0.530261,1.37948,1.46351,0.570151,0.974215,0.513266,1.52976,0.8188,1.89173,0.543367,1.70804,0.511331,1.79913,1.4807,1.28509,1.77838,1.33218,1.03078,1.19793,1.96699,1.40843,1.03943,1.17889,1.88644,0.611496,0.932095,0.640144,1.87763,0.81358,1.93072,1.54052,0.811288,0.633568,0.531228,1.71686,1.77593,1.77183,0.5228,0.988167,1.63381,1.33975,0.904044,0.660944,0.648459,1.21648,1.24184,1.71735,1.10709,0.522968,1.42771,1.16288,1.55932,0.884268,0.550705,1.42803,1.62542,0.615274,1.9891,0.925368,0.838289,1.7142,1.53471,1.11331,1.82982,0.759686,1.04324,0.511939,1.09626,1.67362,0.667073,1.57532,1.28104,0.623215,1.99591,1.11974,0.629461,0.636367,1.7956,1.1731,0.572555,1.0512,1.4601,0.589014,1.83922,1.15626,1.57675,1.74519,1.86435,0.682291,1.33666,1.74496,1.93278,0.553474,1.71064,0.953449,1.55497,1.46369,0.957172,0.961417,1.81417,1.17814,0.753313,1.26745,1.20431,1.33633,1.46986,1.45064,1.56495,1.81822,1.12374,1.64582,0.675799,1.77583,0.693166,1.02924,0.931816,0.776788,1.18227,0.833943,0.640961,1.72761,1.34541,0.691689,0.676852,0.836026,1.28059,0.726181,1.56288,1.69763,1.55288,0.548551,0.707911,1.20656,0.596753,0.937992,1.04849,0.857575,1.88622,1.5654,1.6114,1.008,0.959463,0.715207,1.68199,1.76217,1.92929,1.65678,1.62614,0.567235,0.808791,1.70972,0.937262,1.49098,1.5556,0.657891,0.893784,0.520546,0.819024,0.613991,1.92341,0.580402,1.43741,1.74733,0.806673,1.82873,1.79111,1.16856,1.17567,1.16172,1.12188,1.2357,0.759921,1.30417,1.33452,1.3336,1.71641,1.95436,1.13939,1.09919,0.587166,1.81991,0.986521,1.13344,1.23807,1.09695,0.80013,0.964104,1.07037,1.48472,1.34304,1.54274,0.983428,0.553049,1.25855,0.977885,1.33856,1.55066,1.22992,1.59915,1.39009,0.585234,1.61404,1.74489,0.782699,0.926071,1.27204,1.99547,0.883277,1.28284,1.96093,1.20027,0.690605,1.12217,0.856378,1.60798,1.3719,0.984419,0.66169,1.52843,0.916544,1.42068,0.56876,0.559066,1.79534,1.86584,1.59441,1.68647,0.796936,0.626017,0.623821,0.73482,0.680216,0.732162,1.90514,0.917213,1.43494,1.20417,0.818303,0.522726,1.62337,1.77989,1.33603,1.50305,1.97172,1.2756,1.09107,1.01814,1.47774,0.709092,1.52809,1.41166,0.609784,0.703141,0.834341,1.83074,1.24318,1.67162,0.555279,0.518776,0.550912,0.955249,1.12745,1.8709,1.70268,1.68573,1.51666,1.11326,1.01034,1.01567,1.8269,1.3719,1.38705,0.501924,1.74866,0.912121,1.93253,1.75381,1.14001,1.88134,1.65612,1.93763,1.0071,0.52872,0.938063,1.52115,1.82144,1.99024,1.92076,0.961262,1.18802,0.686889,0.895473,1.52009,0.586577,1.19564,1.21434,1.38363,0.518716,1.41982,0.778705,1.4594,1.07068,0.900121,1.83914,1.8148,1.0636,1.63271,0.728996,1.87586,0.741294,1.59869,1.54929,1.94815,1.12702,1.15566,0.589588,1.63502,1.40839,0.762997,0.985002,1.30968,1.56262,1.92721,1.67849,1.75702,1.78076,0.975077,0.897378,1.49344,1.99492,0.622068,1.26244,1.20664,0.777021,0.827905,0.539219,0.754044,1.83704,1.93827,0.569251,0.567184,1.39025,1.25747,1.97051,0.76881,1.71174,0.892348,1.24721,1.79483,0.818626,0.562773,1.26827,0.800449,0.568142,1.14857,1.60577,1.9954,0.975786,1.3295,0.563143,1.18735,1.9918,1.9156,1.71216,1.41942,0.540325,1.80268,1.83756,1.48382,1.5373,1.0155,1.12612,1.44982,1.00761,1.50522,0.87188,0.572147,1.90746,1.17692,1.96438,1.99062,0.987876,0.838884,1.07312,1.27632,0.865213,1.05934,0.763766,1.26744,0.735513,1.06027,1.61677,1.98098,1.66057,1.46126,0.698713,1.26088,0.695544,0.757668,0.702231,1.21255,1.46197,0.905464,0.940742,1.89614,1.30258,1.57744,0.63443,0.697598,1.13781,0.626151,1.13643,1.4575,0.663487,1.37396,0.578811,0.502113,0.66425,1.78934,1.14284,1.4954,1.18942,1.70032,1.77224,1.91682,1.38093,1.18016,0.816316,1.61177,1.13301,1.1498,1.64678,0.525958,1.50194,0.802129,0.640245,1.41108,0.6746,0.923152,1.18889,0.693672,0.634301,1.55208,1.77411,0.967787,1.8623,1.67885,0.789595,1.62591,1.0137,1.52255,1.94843,1.03928,1.46971,1.14389,1.6152,1.25572,1.45492,1.37998,1.33078,1.53393,1.79419,1.22038,1.81154,0.843091,1.58546,1.59015,0.903343,1.65141,1.88577,1.15166,1.74105,1.70121,1.60304,1.8071,1.21694,1.71715,1.84395,0.893593,0.578752,1.73462,1.36968,0.950257,1.89278,0.509029,1.00725,0.756254,1.8572,0.529285,1.19213,1.74337,1.18761,1.53319,0.671803,1.27821,1.69214,0.609977,0.621528,0.652481,0.918461,1.73107,1.46313,1.98997,1.88987,0.924403,1.65748,1.94221,0.587723,1.62152,0.973169,1.12919,1.30069,0.714671,0.669507,0.838112,0.915019,1.47897,1.10895,1.09644,1.55468,1.97091,0.641434,0.590643,0.870958,1.08372,1.04346,0.796645,1.16841,1.93484,1.70514,1.48038,0.959348,0.693482,1.39671,0.558427,1.48882,1.61469,1.32237,1.421,1.42232,1.09138,1.48352,0.540094,1.33556,0.847699,1.44127,1.75795,0.668803,0.621451,0.985879,1.63843,1.55042,0.705205,1.8711,1.47154,1.40337,1.02747,0.664255,1.58108,0.56156,1.1826,0.501527,1.14092,1.2304,1.70882,0.51899,1.63965,1.39462,1.70665,1.18184,1.74541,0.532508,0.98047,1.32693,0.673629,1.63183,0.502647,1.70344,1.34792,1.62538,1.87497,1.26029,1.49827,1.46644,1.77598,1.03739,1.07317,1.02095,0.751245,1.02781,1.34653,1.12394,1.39158,0.977607,0.975445,0.542139,1.03553,0.638685,1.86519,0.933861,0.812572,1.15325,1.60698,0.967723,1.45416,1.55347,1.92839,1.34762,0.867974,1.02537,1.16437
// };

EventAction::EventAction(RunAction* run, HistoManager* histo)
    :G4UserEventAction(),
    fRunAct(run),
    fHistoManager(histo),
    fEvtNb(-1),
    fHCHCID(-1),fHadCalEdep(),
    fPID(),
    fOP_sc(), fOP_ce(),
    fCrystCoverHCID(-1), fCrystCoverOP(), fCrystFrontCoverHCID(-1), fCrystFrontCoverOP(), fPMTcoverHCID(-1), fPMTcoverOP(),
    fFluxEne(),
    //reuse for Pseudo Sphere
    fFlxHCID(-1),
    fFluxEnergy(-999), fFluxPID(-999),
    fPos(-999),
    hit_HMS(0),

    M_targ(-999.),
    fSmeared_vertex_z(-999.),
    fVertex_z(-999.),

    fEnergyAbs(0.), fEnergyGap(0.),
    fTrackLAbs(0.), fTrackLGap(0.),
    fPrintModulo(0)
{
    fPrintModulo = 100; 
    fHadCalEdep.resize(1080,0.);
    fPID.resize(1080,0),
    fOP_sc.resize(1080,0), fOP_ce.resize(1080,0),
    fCrystCoverOP.resize(1080,0), fCrystFrontCoverOP.resize(1080,0), fPMTcoverOP.resize(1080,0);
    fFluxEne = 0.;

    G4int run_number = dvcsGlobals::run_number;
    dvcs_evt = new TDVCSEvent(run_number);
    calo_evt = new TCaloEvent(run_number);
    L_calo_phot = new TLorentzVector();//If I do not do it, it crashes.....

    dvcs_evt->SetCaloEvent(calo_evt);
    G4cout<<"Calorimter distance (cm): "<<dvcs_evt->GetGeometry()->GetCaloDist()<<G4endl;

    G4cout<<"Caloriemeter angle (deg): "<<dvcs_evt->GetGeometry()->GetCaloTheta()*TMath::RadToDeg()<<G4endl;
    G4int target_gen_proc_type = dvcsGlobals::target_gen_proc_type;
    if( target_gen_proc_type == 0 || target_gen_proc_type == 1 || target_gen_proc_type ==2){
        if(target_gen_proc_type == 0) {M_targ = 0.9383;}
        else if( target_gen_proc_type == 1 ) {M_targ = 0.939565378;}
        else if( target_gen_proc_type == 2 ) {M_targ = 1.875612859;}
    }
    else
    {
        cout<<"Event Action!! Error: wrong target mass "<<endl;
        cout<<"The program is exiting "<<endl;
        exit(1);
    }

    fPx[5] = {-999.};
    fPy[5] = {-999.};
    fPz[5] = {-999.};
    fE[5]  = {-999.};
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

EventAction::~EventAction()
{ }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void EventAction::BeginOfEventAction(const G4Event* evt)
{  
    fEvtNb = evt->GetEventID();

    // Print out running status and time
    G4Run* run = static_cast<G4Run*>( G4RunManager::GetRunManager()->GetNonConstCurrentRun() );
    G4int nOfEvents = run->GetNumberOfEventToBeProcessed();
    G4double perCent = 10.; // status increment in percent

    if(fmod(fEvtNb,double(nOfEvents*perCent*0.01))==0){
        time_t t_now = time(0);
        time_t t_diff = t_now - t_int;
        tm* t_gmtm = gmtime(&t_diff);
        G4double status=(100*(fEvtNb/double(nOfEvents))); 
        std::cout << "=> Event " << fEvtNb << " starts ( "<< status << "%, "<< t_gmtm->tm_hour << ":" <<  t_gmtm->tm_min 
                    << ":" << t_gmtm->tm_sec << " passed )" << std::endl;
    }

    if (fEvtNb%fPrintModulo == 0){
        // initialisation per event
        fEnergyAbs = fEnergyGap = 0.;
        fTrackLAbs = fTrackLGap = 0.;
        fFluxEne = 0.;
    }
    if (fHCHCID==-1){
        //cc-in2p3 does not understand(or geant4.10.02.0 does not understand) "auto"
        //    auto sdManager = G4SDManager::GetSDMpointer();
        G4SDManager* sdManager = G4SDManager::GetSDMpointer();
        fHCHCID = sdManager->GetCollectionID("HadCalorimeter/HadCalorimeterColl");//B2/TrackerChamberSD is the SDname, TrackerHitsCollection is the collection ID.
    }

    if (fCrystCoverHCID==-1){
        G4SDManager* sdManager = G4SDManager::GetSDMpointer();
        fCrystCoverHCID = sdManager->GetCollectionID("CrystalCover/CrystalCoverColl");
    }

    if (fCrystFrontCoverHCID==-1){
        G4SDManager* sdManager = G4SDManager::GetSDMpointer();
        fCrystFrontCoverHCID = sdManager->GetCollectionID("CrystalFrontCover/CrystalFrontCoverColl");
    }

    if (fPMTcoverHCID==-1){
        G4SDManager* sdManager = G4SDManager::GetSDMpointer();
        fPMTcoverHCID = sdManager->GetCollectionID("PMTcover/PMTcoverColl");
    }

    //reuse for Pseudo Sphere

    if (fFlxHCID==-1){
        G4SDManager* sdManager = G4SDManager::GetSDMpointer();
        fFlxHCID = sdManager->GetCollectionID("Flux/FluxColl");//B2/TrackerChamberSD is the SDname, TrackerHitsCollection is the collection ID.
    }

    if(dvcsGlobals::hit_HMS_CALO_flag ){
        fPx[4] = 0.;
        fPy[4] = 0.;
        fPz[4] = 0.;
        fE[4] = 0.;
    }
    fHistoManager->SetEvtNb(fEvtNb);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void EventAction::EndOfEventAction(const G4Event* evt)
{
    //accumulates statistic
    //
    fRunAct->fillPerEvent(fEnergyAbs, fEnergyGap, fTrackLAbs, fTrackLGap);
  
    //cc-in2p3 does not understand(or geant4.10.02.0 does not understand) "auto"
    //  auto hce = evt->GetHCofThisEvent();
    G4HCofThisEvent* hce = evt->GetHCofThisEvent();
    if (!hce) {
        G4ExceptionDescription msg;
        msg << "No hits collection of this event found." << G4endl; 
        G4Exception("EventAction::EndOfEventAction()", "Code001", JustWarning, msg);
        return;
    }

    // Get hits collections
    //cc-in2p3 does not understand(or geant4.10.02.0 does not understand) "auto" 
    //  auto hcHC 
    B5HadCalorimeterHitsCollection *hcHC = static_cast<B5HadCalorimeterHitsCollection*>(hce->GetHC(fHCHCID));
    if (!hcHC) {
        G4ExceptionDescription msg;
        msg << "Some of hits collections of this event not found." << G4endl; 
        G4Exception("EventAction::EndOfEventAction()", "Code001", JustWarning, msg);
        return;
    }   

    CrystalCoverHitsCollection *CrystCoverHC = static_cast<CrystalCoverHitsCollection*>(hce->GetHC(fCrystCoverHCID));
    if (!CrystCoverHC) {
        G4ExceptionDescription msg;
        msg << "Some of hits collections of this event not found." << G4endl; 
        G4Exception("EventAction::EndOfEventAction()", "Code001", JustWarning, msg);
        return;
    }   
    CrystalFrontCoverHitsCollection *CrystFrontCoverHC = static_cast<CrystalFrontCoverHitsCollection*>(hce->GetHC(fCrystFrontCoverHCID));
    if (!CrystFrontCoverHC) {
        G4ExceptionDescription msg;
        msg << "Some of hits collections of this event not found." << G4endl; 
        G4Exception("EventAction::EndOfEventAction()", "Code001", JustWarning, msg);
        return;
    }   
    PMTcoverHitsCollection *PMTcoverHC = static_cast<PMTcoverHitsCollection*>(hce->GetHC(fPMTcoverHCID));
    if (!PMTcoverHC) {
        G4ExceptionDescription msg;
        msg << "Some of hits collections of this event not found." << G4endl; 
        G4Exception("EventAction::EndOfEventAction()", "Code001", JustWarning, msg);
        return;
    }   

    // HCEnergy
    for (G4int i=0;i<1080;i++){
        B5HadCalorimeterHit* hit = (*hcHC)[i];
        G4double eDep = hit->GetEdep();//total energy deposition of each crystals
        G4int PID = hit->GetPID();
        // SetEnrgyandPID must be done for all 1080 crystals.
        // If not, the edep from last events with edep>0 will be stored again. 
        fHadCalEdep[i] = eDep*1.04;//Compensate lost 4% of the total energy of the shower at the end of the crystal.
        fPID[i] = PID;

        CrystalCoverHit* CChit = (*CrystCoverHC)[i];
        CrystalFrontCoverHit* CFChit = (*CrystFrontCoverHC)[i];
        PMTcoverHit* PMTChit = (*PMTcoverHC)[i];
        G4double sc = hit->GetOPInt_sc();
        G4double ce = hit->GetOPInt_ce();
        fOP_sc[i] = sc;
        fOP_ce[i] = ce;

        //No. of OP reflected at the side of the crystal wrapper.
        G4int CrystalCoverOP = CChit->GetOPInt();  
        fCrystCoverOP[i] = CrystalCoverOP;
        //No. of OP reflected at the front of the crystal wrapper
        G4int CrystalFrontCoverOP = CFChit->GetOPInt();  
        fCrystFrontCoverOP[i] = CrystalFrontCoverOP;
        //No. of OP arrived at the PMT cover
        G4int PMTcoverOP = PMTChit->GetOPInt();  
        fPMTcoverOP[i] = PMTcoverOP;

        fHistoManager->SetEnergyandPID( i, fPID[i], fHadCalEdep[i], fOP_sc[i], fOP_ce[i], fCrystCoverOP[i], fCrystFrontCoverOP[i], fPMTcoverOP[i]);
    }
    // fHistoManager->FillNtuple();

    if( dvcsGlobals::hit_HMS_CALO_flag ){
        G4double Ebeam = dvcsGlobals::Ebeam; 
        G4int ObjectNumber=TProcessID::GetObjectCount(); 

        for (G4int i = 0 ; i < 1080 ; i++){
            B5HadCalorimeterHit* hit = (*hcHC)[i];
            G4double eDep = hit->GetEdep();//total energy deposition of each crystals       
            TCaloBlock* block=calo_evt->AddBlock(i);
            //energy and time
            //Currently, no time info in Geant4 simulation. default as 0 for now.
            block->AddPulse(eDep*(1e-3)*(1.04), 0.);//energy (GeV) and time (ns) //Compensate lost 4% of the total energy of the shower at the end of the crystal.
	    }

        dvcs_evt->SetVertex(0., 0., fSmeared_vertex_z);//[cm] unit

        calo_evt->TriggerSim(0.5);//0.5 GeV // if too many blocks in the event
        calo_evt->DoClustering(-3., 3.);//-3ns to +3ns windows
        G4int Nb_clust = calo_evt->GetNbClusters();
        for(G4int i = 0 ; i < Nb_clust ; i++ ){
            TCaloCluster *clus = calo_evt->GetCluster(i);
            clus->Analyze(1., -3., 3.);//"true||false", "time_min(ns)", "time_max(ns)", "weight"
            *L_calo_phot = dvcs_evt->GetPhoton(i);
            if(Nb_clust) fHistoManager->SetClusterPositionandMomentum(evt->GetEventID(), clus->GetX(), clus->GetY(), L_calo_phot->E(), L_calo_phot->Px(), L_calo_phot->Py(), L_calo_phot->Pz());
        }
        if(Nb_clust >= 1 && hit_HMS){
            G4double clust_x   = calo_evt->GetCluster(0)->GetX();
            G4double clust_y   = calo_evt->GetCluster(0)->GetY();
            G4double clust_ene = calo_evt->GetCluster(0)->GetE();
            G4int clust_size   = calo_evt->GetCluster(0)->GetClusSize();
            fHistoManager->SetCaloData(clust_ene, clust_x, clust_y, clust_size);

            G4double phot_px = L_calo_phot->Px();//GeV
            G4double phot_py = L_calo_phot->Py();
            G4double phot_pz = L_calo_phot->Pz();

	        fHistoManager->SetPhotRec(phot_px, phot_py, phot_pz); //Set Reconstructed photon kinematics: Px, Py, Pz

            // fHistoManager->SetClusterPositionandMomentum(evt->GetEventID(), clust_x, clust_y, clust_ene, phot_px, phot_py, phot_pz);
            // fHistoManager->FillNtupleClust();

            TVector3 k(0, 0, Ebeam);
            TVector3 kp(fPx[4], fPy[4], fPz[4]);
            TVector3 qp(phot_px, phot_py, phot_pz);

            G4double Q2 = 2*k.Mag()*kp.Mag()*(1 - cos(k.Angle(kp)) ); // Q2 = 2p1p2*(1 - cos(tehta_12))

            TVector3 q = k - kp;

            TVector3 v1=q.Cross(kp);
            TVector3 v2=q.Cross(qp);
            G4double fphi=v1.Angle(v2);
            if(q.Dot(v1.Cross(v2))<0) fphi=2.*TMath::Pi()-fphi;

            G4double cos=(q.Dot(qp))/(q.Mag()*qp.Mag());
            G4double nu=Ebeam-kp.Mag();
            G4double tM=(Q2*M_targ+2.*nu*M_targ*(nu-sqrt(nu*nu+Q2)*cos))/(sqrt(nu*nu+Q2)*cos-nu-M_targ);

            G4double xB = Q2/(2*M_targ*nu);

            fHistoManager->SetKineRec(tM, xB, Q2, fphi);

            TLorentzVector Lb, Lp, Lgp, Lkp, L_mis;
            Lb.SetPxPyPzE(0, 0, Ebeam, Ebeam);
            Lp.SetPxPyPzE(0, 0, 0, M_targ);
            Lkp.SetPxPyPzE(fPx[4], fPy[4], fPz[4], fE[4]);
            Lgp = *L_calo_phot;

            G4double mm2 = (Lb + Lp - Lgp - Lkp).M2();

            fHistoManager->SetClustW2(mm2);

            fHistoManager->FillNtupleDVCS();
        }

        hit_HMS = false;
        calo_evt->Reset();
        TProcessID::SetObjectCount(ObjectNumber);
    }

    // Get hits collections
    //cc-in2p3 does not understand(or geant4.10.02.0 does not understand) "auto" 
    //  auto hcHC 
    FluxHitsCollection *fluxHC = static_cast<FluxHitsCollection*>(hce->GetHC(fFlxHCID));
    if (!fluxHC) {
        G4ExceptionDescription msg;
        msg << "Some of hits collections of this event not found." << G4endl; 
        G4Exception("EventAction::EndOfEventAction()", "Code001", JustWarning, msg);
        return;
    }   
    FluxHit* fluxhit = (*fluxHC)[0];
    G4double energy = fluxhit->GetEdep();//energy of the particle passing through the volume
    G4int fluxPID = fluxhit->GetPID();
    G4ThreeVector position = fluxhit->GetPos();
    fFluxEnergy = energy;
    fFluxPID = fluxPID;
    fPos = position;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void EventAction::DefinePrimaries(TLorentzVector* L_elec_init, TLorentzVector* L_elec_scat, TLorentzVector* L_phot_final, TLorentzVector* L_prot_final, G4double vertex_x, G4double vertex_y, G4double vertex_z)
{
    fPx[0] = L_elec_init->Px(); fPx[1] = L_elec_scat->Px(); fPx[2] = L_phot_final->Px(); fPx[3] = L_prot_final->Px();
    fPy[0] = L_elec_init->Py(); fPy[1] = L_elec_scat->Py(); fPy[2] = L_phot_final->Py(); fPy[3] = L_prot_final->Py();
    fPz[0] = L_elec_init->Pz(); fPz[1] = L_elec_scat->Pz(); fPz[2] = L_phot_final->Pz(); fPz[3] = L_prot_final->Pz();
    fE[0]  = L_elec_init->E();  fE[1]  = L_elec_scat->E();  fE[2]  = L_phot_final->E();  fE[3]  = L_prot_final->E();

    G4double HMS_angle = dvcsGlobals::HMS_angle;
    G4double Ebeam = dvcsGlobals::Ebeam;
    fVertex_x = vertex_x;//[cm]
    fVertex_y = vertex_y;//[cm]
    fVertex_z = vertex_z;//[cm]
    //Is it okay to set seed here?
    TRandom2 rand2;
    rand2.SetSeed(0);// "0" automatically generate seed values which are different every time by using TRandom3
    fSmeared_vertex_z = fVertex_z + rand2.Gaus(0, 1.0/sin(HMS_angle))*0.1; // 0.1 is for mm->cm //Hall C HMS Target Vertex Recoonstruction Accuracy : 1.0 mm

    fHistoManager->SetVertexz(fSmeared_vertex_z, fVertex_z);
    fHistoManager->SetVertexGen(fVertex_x, fVertex_y, fVertex_z);

    G4double beam_px = 0.;
    G4double beam_py = 0.;
    G4double beam_pz = Ebeam;

    fHistoManager->SetInitElecRec(beam_px, beam_py, beam_pz);
    fHistoManager->SetInitElecGen(fPx[0], fPy[0], fPz[0]);
    fHistoManager->SetScatElecGen(fPx[1], fPy[1], fPz[1]);
    fHistoManager->SetPhotGen(fPx[2], fPy[2], fPz[2]);

    //===========Calculate Q2, t, phi from DVCS event generator=========

    TVector3 mom_init_elec_gen(0, 0, fE[0]);//? why not (fPx[0], fPy[0], fPz[0])?

    TVector3 mom_scat_elec_gen(fPx[1], fPy[1], fPz[1]);
    TVector3 mom_final_phot_gen(fPx[2], fPy[2], fPz[2]);

    G4double Q2_gen = 2*mom_init_elec_gen.Mag()*mom_scat_elec_gen.Mag()*(1 - cos(mom_init_elec_gen.Angle(mom_scat_elec_gen)) ); // Q2 = 2p1p2*(1 - cos(tehta_12))
    TVector3 mom_virt_phot_gen = mom_init_elec_gen - mom_scat_elec_gen;

    TVector3 v1_gen = mom_virt_phot_gen.Cross(mom_scat_elec_gen);
    TVector3 v2_gen = mom_virt_phot_gen.Cross(mom_final_phot_gen);
    G4double phi_gen = v1_gen.Angle(v2_gen);
    if(mom_virt_phot_gen.Dot(v1_gen.Cross(v2_gen))<0) phi_gen = 2.*TMath::Pi()-phi_gen;

    G4double cos_gen = (mom_virt_phot_gen.Dot(mom_final_phot_gen))/(mom_virt_phot_gen.Mag()*mom_final_phot_gen.Mag());
    G4double nu_gen = fE[0]-mom_scat_elec_gen.Mag();
    G4double xB_gen = Q2_gen/(2*M_targ*nu_gen);
    //G4double tM_gen=(Q2_gen*M_targ+2.*nu_gen*M_targ*(nu_gen-sqrt(nu_gen*nu_gen+Q2_gen)*cos_gen))/(sqrt(nu_gen*nu_gen+Q2_gen)*cos_gen-nu_gen-M_targ);
    G4double tM_gen = 2*M_targ*(M_targ - fE[3]);

    fHistoManager->SetKineGen(tM_gen, xB_gen, Q2_gen, phi_gen);


    //============Calculate R_value from DVCS event generator===========

    //  G4double HMS_angle= dvcsGlobals::HMS_angle;  //23.91*TMath::Pi()/180.; // KINEMATICS 3

    G4double mom_central= dvcsGlobals::HMS_momentum;

    //HMS_angle
    G4int run_number = dvcsGlobals::run_number;
    G4double ry = -0.01*fVertex_z*TMath::Sin(TMath::ATan2(fPx[1],fPz[1])); // 0.01 to convert cm to meters
    G4double rdp= (sqrt(fPx[1]*fPx[1]+fPy[1]*fPy[1]+fPz[1]*fPz[1])-mom_central)/mom_central;
    G4double rtheta = TMath::ATan2(-fPy[1],TMath::Sqrt(fPx[1]*fPx[1]+fPz[1]*fPz[1]));
    G4double rphi   = TMath::ATan2(fPx[1],fPz[1]) - HMS_angle;

    //G4double r_val_gen = dvcs_event->rfunction(ry,rdp,rtheta,rphi);
    G4double r_val_gen = RFunction(run_number, rtheta, rdp, rphi, ry);//Alexa's RFunction
    fHistoManager->SetRvalGen(r_val_gen);
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void EventAction::DefineWeights(G4double PSF, G4double X_sum, G4double X_diff, G4double X_BH)
{
    fPSF = PSF;
    fX_sum = X_sum;
    fX_diff = X_diff;
    fX_BH = X_BH;

    fHistoManager->SetWeights(fPSF, fX_sum, fX_diff, fX_BH);
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void EventAction::DefineHMS_elec(G4ThreeVector mom, G4double ene)
{
    fPx[4] = mom.getX();
    fPy[4] = mom.getY();
    fPz[4] = mom.getZ();
    fE[4]  = ene;

    fHistoManager->SetScatElecRec(fPx[4], fPy[4], fPz[4]);
    hit_HMS = true;

    //============Calculate R_value from Detected electron on HMS===========

    G4double HMS_angle = dvcsGlobals::HMS_angle;
    G4double pcentral= dvcsGlobals::HMS_momentum; // KINEMATICS 3

    //HMS_angle
    G4int run_number = dvcsGlobals::run_number;
    G4double ry = -0.01*fSmeared_vertex_z*TMath::Sin(TMath::ATan2(fPx[4],fPz[4]));  // 0.01 to convert cm to meters
    G4double rdp= (sqrt(fPx[4]*fPx[4]+fPy[4]*fPy[4]+fPz[4]*fPz[4])-pcentral)/pcentral;
    G4double rtheta = TMath::ATan2(-fPy[4],TMath::Sqrt(fPx[4]*fPx[4]+fPz[4]*fPz[4]));
    G4double rphi   = TMath::ATan2(fPx[4],fPz[4]) - HMS_angle;

    //G4double r_val_rec = dvcs_event->rfunction(ry,rdp,rtheta,rphi);
    G4double r_val_rec = RFunction(run_number,rtheta,rdp,rphi,ry);//Alexa's RFunction
    fHistoManager->SetRvalRec(r_val_rec);
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
G4int EventAction::GetEventNb()
{
    return fEvtNb;
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
