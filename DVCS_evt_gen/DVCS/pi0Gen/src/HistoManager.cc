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
/// \file analysis/AnaEx02/src/HistoManager.cc
/// \brief Implementation of the HistoManager class
//
// $Id: HistoManager.cc 92374 2015-08-31 08:52:09Z gcosmo $
// GEANT4 tag $Name: geant4-09-04 $
// 
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include <TH1D.h>
#include <TFile.h>
#include <TTree.h>
#include <CLHEP/Units/SystemOfUnits.h>

#include "HistoManager.hh"
#include "G4UnitsTable.hh"

#include "G4SystemOfUnits.hh"
#include "G4PhysicalConstants.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

HistoManager::HistoManager()
    :fRootFile(0),
    fNtuple(0),
    fNtuple_DVCS(0),
    fNtuple_Clust(0),
    fNtuple2(0),

    fNtuple_gamma(0),
    fNtuple_pos(0),
    fNtuple_elec(0),
    fNtuple_unknown(0),

    fNtuple_OptFiber_gamma(0),
    fNtuple_OptFiber_pos(0),
    fNtuple_OptFiber_elec(0),
    fNtuple_OptFiber_unknown(0),

    // Modified for pi0
    fEvtNb(-1),
    fClust_Ene1(-999.),
    fClust_X1(-999.),
    fClust_Y1(-999.),
    fClust_size1(-1),
    fClust_Ene2(-999.),
    fClust_X2(-999.),
    fClust_Y2(-999.),
    fClust_size2(-1),
    fClust_W2(-999.),
    fPSF(-999.),
    fRIE_Px(-999.),
    fRIE_Py(-999.),
    fRIE_Pz(-999.),
    fGIE_Px(-999.),
    fGIE_Py(-999.),
    fGIE_Pz(-999.),
    fRSE_Px(-999.),
    fRSE_Py(-999.),
    fRSE_Pz(-999.),
    fGSE_Px(-999.),
    fGSE_Py(-999.),
    fGSE_Pz(-999.),
    fRP_Px1(-999.),
    fRP_Py1(-999.),
    fRP_Pz1(-999.),
    fGP_Px1(-999.),
    fGP_Py1(-999.),
    fGP_Pz1(-999.),
    fRP_Px2(-999.),
    fRP_Py2(-999.),
    fRP_Pz2(-999.),
    fGP_Px2(-999.),
    fGP_Py2(-999.),
    fGP_Pz2(-999.),
    fRM(-999.),
    fGM(-999.),
    fRV_Z(-999.),
    fGV_Z(-999.),
    fRt(-999.),
    fGt(-999.),
    fRxB(-999.),
    fGxB(-999.),
    fRQ2(-999.),
    fGQ2(-999.),
    fRphi(-999.),
    fGphi(-999.),
    fX_sum(-999.),
    fX_diff(-999.),
    fX_BH(-999.),
    fRr_val(-999.),
    fGr_val(-999.),

    fEvtNb_(-1),
    fClust_X_(-999.),
    fClust_Y_(-999.),
    fClust_Ene_(-999.),
    fClust_Px_(-999.),
    fClust_Py_(-999.),
    fClust_Pz_(-999.),

    fFluxEnergy_gamma(-999.),
    fTheta_gamma(-999.), 
    fPhi_gamma(-999.), 
    fFluxEnergy_pos(-999.),
    fTheta_pos(-999.), 
    fPhi_pos(-999.), 
    fFluxEnergy_elec(-999.),
    fTheta_elec(-999.), 
    fPhi_elec(-999.), 
    fFluxEnergy_unknown(-999.),
    fFluxPID_unknown(-999),
    fTheta_unknown(-999.), 
    fPhi_unknown(-999.),

    fFluxEvtNb(-1),

    fFluxMomX_OptFiber_gamma(-999.),
    fFluxMomY_OptFiber_gamma(-999.),
    fFluxMomZ_OptFiber_gamma(-999.),
    fFluxMomX_OptFiber_pos(-999.),
    fFluxMomY_OptFiber_pos(-999.),
    fFluxMomZ_OptFiber_pos(-999.),
    fFluxMomX_OptFiber_elec(-999.),
    fFluxMomY_OptFiber_elec(-999.),
    fFluxMomZ_OptFiber_elec(-999.),
    fFluxMomX_OptFiber_unknown(-999.),
    fFluxMomY_OptFiber_unknown(-999.),
    fFluxMomZ_OptFiber_unknown(-999.),

    fFluxEnergy_OptFiber_gamma(-999.),
    fGlobalX_OptFiber_gamma(-999.),
    fGlobalY_OptFiber_gamma(-999.),
    fGlobalZ_OptFiber_gamma(-999.),
    fLocalX_OptFiber_gamma(-999.),
    fLocalY_OptFiber_gamma(-999.),
    fLocalZ_OptFiber_gamma(-999.),
    fFluxEnergy_OptFiber_pos(-999.),
    fGlobalX_OptFiber_pos(-999.),
    fGlobalY_OptFiber_pos(-999.),
    fGlobalZ_OptFiber_pos(-999.),
    fLocalX_OptFiber_pos(-999.),
    fLocalY_OptFiber_pos(-999.),
    fLocalZ_OptFiber_pos(-999.),
    fFluxEnergy_OptFiber_elec(-999.),
    fGlobalX_OptFiber_elec(-999.),
    fGlobalY_OptFiber_elec(-999.),
    fGlobalZ_OptFiber_elec(-999.),
    fLocalX_OptFiber_elec(-999.),
    fLocalY_OptFiber_elec(-999.),
    fLocalZ_OptFiber_elec(-999.),
    fFluxEnergy_OptFiber_unknown(-999.),
    fFluxPID_OptFiber_unknown(-999),
    fGlobalX_OptFiber_unknown(-999.),
    fGlobalY_OptFiber_unknown(-999.),
    fGlobalZ_OptFiber_unknown(-999.),
    fLocalX_OptFiber_unknown(-999.),
    fLocalY_OptFiber_unknown(-999.),
    fLocalZ_OptFiber_unknown(-999.)
{
    fEdep[MaxNtuple] = {0.};
    fPID[MaxNtuple] = {0};
    fOP_sc[MaxNtuple] = {0};
    fOP_ce[MaxNtuple] = {0};
    fOP_cover[MaxNtuple] = {0};
    fOP_frontcover[MaxNtuple] = {0};
    fOP_pmtcover[MaxNtuple] = {0};

    fFluxEne[MaxNtuple] = {0.};
    fFluxPID[MaxNtuple] = {0};
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

HistoManager::~HistoManager()
{
    if (fRootFile) delete fRootFile;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void HistoManager::Book(G4String fileName)
{ 
    // Creating a tree container to handle histograms and ntuples.
    // This tree is associated to an output file.
    //
    fRootFile = new TFile(fileName,"RECREATE");
    if (! fRootFile) {
        G4cout << " HistoManager::Book :" 
               << " problem creating the ROOT TFile "
               << G4endl;
        return;
    }
  
    // fNtuple = new TTree("t","Energy deposition and OP in crystas");

    //Energy deposition & optical photons for each event
    // fNtuple->Branch("evtNb", &fEvtNb, "Event Number/I");
    // fNtuple->Branch("edep", fEdep, "energy_deposition[1080]/D");
    //temporarily disabled, no op physics
    // fNtuple->Branch("sc", fOP_sc, "scintillated OP[1080]/I");
    // fNtuple->Branch("ce", fOP_ce, "cerenkov OP[1080]/I");
    // fNtuple->Branch("op_cover", fOP_cover, "OP on the side of the crystal wrapper[1080]/I");
    // fNtuple->Branch("op_frontcover", fOP_frontcover, "OP on the front side of the crystal wrapper[1080]/I");
    // fNtuple->Branch("op_pc", fOP_pmtcover, "OP arrived at the pmt cover[1080]/I");

    //pi0
    fNtuple_DVCS = new TTree("t_dvcs", "pi0 events generated and reconstructed");
    fNtuple_DVCS->Branch("evtNb", &fEvtNb, "Event Number/I");
    fNtuple_DVCS->Branch("edep", fEdep, "energy_deposition[1080]/D");

    // fNtuple_DVCS->Branch("clust_ene1", &fClust_Ene1, "Cluster Energy1/D");
    // fNtuple_DVCS->Branch("clust_x1", &fClust_X1, "Cluster X1/D");
    // fNtuple_DVCS->Branch("clust_y1", &fClust_Y1, "Cluster Y1/D");
    // fNtuple_DVCS->Branch("clust_size1", &fClust_size1, "Cluster size1/I");

    // fNtuple_DVCS->Branch("clust_ene2", &fClust_Ene2, "Cluster Energy2/D");
    // fNtuple_DVCS->Branch("clust_x2", &fClust_X2, "Cluster X2/D");
    // fNtuple_DVCS->Branch("clust_y2", &fClust_Y2, "Cluster Y2/D");
    // fNtuple_DVCS->Branch("clust_size2", &fClust_size2, "Cluster size2/I");

    // fNtuple_DVCS->Branch("clust_W2", &fClust_W2, "Missing mass squared from Clustering/D");
    fNtuple_DVCS->Branch("psf", &fPSF, "Phase Space Factor from pi0 gen/D");

    fNtuple_DVCS->Branch("RIE_px", &fRIE_Px, "Initial electron Px from Geant4, Actually, it is beam energy/D");
    fNtuple_DVCS->Branch("RIE_py", &fRIE_Py, "Initial electron Py from Geant4, Actually, it is beam energy/D");
    fNtuple_DVCS->Branch("RIE_pz", &fRIE_Pz, "Initial electron Pz from Geant4, Actually, it is beam energy/D");
    fNtuple_DVCS->Branch("GIE_px", &fGIE_Px, "Initial electron Px from pi0 gen/D");
    fNtuple_DVCS->Branch("GIE_py", &fGIE_Py, "Initial electron Py from pi0 gen/D");
    fNtuple_DVCS->Branch("GIE_pz", &fGIE_Pz, "Initial electron Pz from pi0 gen/D");

    // fNtuple_DVCS->Branch("RSE_px", &fRSE_Px, "Scattered electron Px detected in HMS window in Geant4/D");
    // fNtuple_DVCS->Branch("RSE_py", &fRSE_Py, "Scattered electron Py detected in HMS window in Geant4/D");
    // fNtuple_DVCS->Branch("RSE_pz", &fRSE_Pz, "Scattered electron Pz detected in HMS window in Geant4/D");
    fNtuple_DVCS->Branch("GSE_px", &fGSE_Px, "Scattered electron Px from pi0 gen/D");
    fNtuple_DVCS->Branch("GSE_py", &fGSE_Py, "Scattered electron Py from pi0 gen/D");
    fNtuple_DVCS->Branch("GSE_pz", &fGSE_Pz, "Scattered electron Pz from pi0 gen/D");
    // fNtuple_DVCS->Branch("RP_px1", &fRP_Px1, "Real proton1 Px reconstructed from clustering/D");
    // fNtuple_DVCS->Branch("RP_py1", &fRP_Py1, "Real proton1 Py reconstructed from clustering/D");
    // fNtuple_DVCS->Branch("RP_pz1", &fRP_Pz1, "Real proton1 Pz reconstructed from clustering/D");
    fNtuple_DVCS->Branch("GP_px1", &fGP_Px1, "Real proton1 Px from pi0 gen/D");
    fNtuple_DVCS->Branch("GP_py1", &fGP_Py1, "Real proton1 Py from pi0 gen/D");
    fNtuple_DVCS->Branch("GP_pz1", &fGP_Pz1, "Real proton1 Pz from pi0 gen/D");
    // fNtuple_DVCS->Branch("RP_px2", &fRP_Px2, "Real proton2 Px reconstructed from clustering/D");
    // fNtuple_DVCS->Branch("RP_py2", &fRP_Py2, "Real proton2 Py reconstructed from clustering/D");
    // fNtuple_DVCS->Branch("RP_pz2", &fRP_Pz2, "Real proton2 Pz reconstructed from clustering/D");
    fNtuple_DVCS->Branch("GP_px2", &fGP_Px2, "Real proton2 Px from pi0 gen/D");
    fNtuple_DVCS->Branch("GP_py2", &fGP_Py2, "Real proton2 Py from pi0 gen/D");
    fNtuple_DVCS->Branch("GP_pz2", &fGP_Pz2, "Real proton2 Pz from pi0 gen/D");
    // fNtuple_DVCS->Branch("RM", &fRM, "Pi0 mass reconstructed from clustering/D");
    
    fNtuple_DVCS->Branch("GV_x", &fGV_X, "Vertex position X from pi0 gen/D");
    fNtuple_DVCS->Branch("GV_y", &fGV_Y, "Vertex position Y from pi0 gen/D");
    fNtuple_DVCS->Branch("GV_z", &fGV_Z, "Vertex position Z from pi0 gen/D");
    // fNtuple_DVCS->Branch("RV_z", &fRV_Z, "Vertex position Z smeared from Geant4/D");//Considereing HMS resolution.

    fNtuple_DVCS->Branch("Gt", &fGt, "t from pi0 gen/D");
    // fNtuple_DVCS->Branch("Rt", &fRt, "t from Geant4/D");
    fNtuple_DVCS->Branch("GxB", &fGxB, "xB from pi0 gen/D");
    // fNtuple_DVCS->Branch("RxB", &fRxB, "xB from Geant4/D");
    fNtuple_DVCS->Branch("GQ2", &fGQ2, "Q2 from pi0 gen/D");
    // fNtuple_DVCS->Branch("RQ2", &fRQ2, "Q2 from Geant4/D");
    fNtuple_DVCS->Branch("Gphi", &fGphi, "phi from pi0 gen/D");
    // fNtuple_DVCS->Branch("Rphi", &fRphi, "phi from Geant4/D");
    fNtuple_DVCS->Branch("X_sum", &fX_sum, "XSecSum(0) from pi0 gen/D");
    fNtuple_DVCS->Branch("X_diff", &fX_diff, "XSecDif() from pi0 gen/D");
    fNtuple_DVCS->Branch("X_BH", &fX_BH, "XSecSum(1) from pi0 gen/D");
    // fNtuple_DVCS->Branch("Gr_val", &fGr_val, "RFunction using values from pi0 gen/D");
    // fNtuple_DVCS->Branch("Rr_val", &fRr_val, "RFunction using values from Geant4/D");

    G4cout << "\n----> Output file is open in " << fileName << G4endl;

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void HistoManager::Save()
{ 
    if (! fRootFile) return;
    fRootFile->Write();       // Writing the histograms to the file
    fRootFile->Close();       // and closing the tree (and the file)
  
    G4cout << "\n----> Histograms and ntuples are saved\n" << G4endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void HistoManager::FillNtuple()
{
    fNtuple->Fill();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void HistoManager::FillNtupleDVCS()
{
    fNtuple_DVCS->Fill();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void HistoManager::FillNtupleClust()
{
    // fNtuple_Clust->Fill();
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void HistoManager::FillNtuple2()
{
    // fNtuple2->Fill();
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void HistoManager::FillNtupleFluxSphere(G4int FluxPID)
{
    // if(FluxPID == 22)       fNtuple_gamma->Fill();
    // else if(FluxPID == -11) fNtuple_pos->Fill();
    // else if(FluxPID ==  11) fNtuple_elec->Fill();
    // else                    fNtuple_unknown->Fill();
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void HistoManager::FillNtupleFluxOptFiber(G4int FluxPID)
{
    // if(FluxPID == 22)       fNtuple_OptFiber_gamma->Fill();
    // else if(FluxPID == -11) fNtuple_OptFiber_pos->Fill();
    // else if(FluxPID ==  11) fNtuple_OptFiber_elec->Fill();
    // else                    fNtuple_OptFiber_unknown->Fill();
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void HistoManager::SetEnergyandPID(G4int id, G4int PID, G4double edep, G4int sc, G4int ce, G4int opc, G4int opfc, G4int oppc)
{
    fEdep[id] = edep;
  
    //temporarily disabled. no op physics
    // fPID[id] = PID;

    // fOP_sc[id] = sc;
    // fOP_ce[id] = ce;
    // fOP_cover[id] = opc;
    // fOP_frontcover[id] = opfc;
    // fOP_pmtcover[id] = oppc;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void HistoManager::SetEvtNb(G4int evtNb)
{
    fEvtNb = evtNb;
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void HistoManager::SetCaloData1(G4double clust_ene, G4double clust_x, G4double clust_y, G4int clust_size)
{
    fClust_Ene1 = clust_ene;
    fClust_X1 = clust_x;
    fClust_Y1 = clust_y;
    fClust_size1 = clust_size;
}

void HistoManager::SetCaloData2(G4double clust_ene, G4double clust_x, G4double clust_y, G4int clust_size)
{
    fClust_Ene2 = clust_ene;
    fClust_X2 = clust_x;
    fClust_Y2 = clust_y;
    fClust_size2 = clust_size;
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void HistoManager::SetClustW2(G4double W2)
{
    fClust_W2 = W2;
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void HistoManager::SetInitElecRec(G4double px, G4double py, G4double pz)
{
    fRIE_Px = px;
    fRIE_Py = py;
    fRIE_Pz = pz;
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void HistoManager::SetInitElecGen(G4double px, G4double py, G4double pz)
{
    fGIE_Px = px;
    fGIE_Py = py;
    fGIE_Pz = pz;
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void HistoManager::SetScatElecRec(G4double px, G4double py, G4double pz)
{
    fRSE_Px = px;
    fRSE_Py = py;
    fRSE_Pz = pz;
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void HistoManager::SetScatElecGen(G4double px, G4double py, G4double pz)
{
    fGSE_Px = px;
    fGSE_Py = py;
    fGSE_Pz = pz;
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void HistoManager::SetPhotRec1(G4double px, G4double py, G4double pz)
{
    fRP_Px1 = px;
    fRP_Py1 = py;
    fRP_Pz1 = pz;
}
void HistoManager::SetPhotRec2(G4double px, G4double py, G4double pz)
{
    fRP_Px2 = px;
    fRP_Py2 = py;
    fRP_Pz2 = pz;
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void HistoManager::SetPhotGen1(G4double px, G4double py, G4double pz)
{
    fGP_Px1 = px;
    fGP_Py1 = py;
    fGP_Pz1 = pz;
}
void HistoManager::SetPhotGen2(G4double px, G4double py, G4double pz)
{
    fGP_Px2 = px;
    fGP_Py2 = py;
    fGP_Pz2 = pz;
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void HistoManager::SetMpi0Gen(G4double m)
{
    fGM = m;
}
void HistoManager::SetMpi0Rec(G4double m)
{
    fRM = m;
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void HistoManager::SetVertexz(G4double rv_z, G4double gv_z)
{
    fRV_Z = rv_z;
    fGV_Z = gv_z;
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void HistoManager::SetVertexGen(G4double gv_x, G4double gv_y, G4double gv_z)
{
  fGV_X = gv_x;
  fGV_Y = gv_y;
  fGV_Z = gv_z;
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void HistoManager::SetKineRec(G4double t, G4double xB, G4double Q2, G4double phi)
{
    fRt = t;
    fRxB = xB;
    fRQ2 = Q2;
    fRphi = phi;
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void HistoManager::SetKineGen(G4double t, G4double xB, G4double Q2, G4double phi)
{
    fGt = t;
    fGxB = xB;
    fGQ2 = Q2;
    fGphi = phi;
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void HistoManager::SetWeights(G4double PSF, G4double X_sum, G4double X_diff, G4double X_BH)
{
    fPSF = PSF;
    fX_sum = X_sum;
    fX_diff = X_diff;
    fX_BH = X_BH;
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void HistoManager::SetRvalRec(G4double r_val)
{
    fRr_val = r_val;
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void HistoManager::SetRvalGen(G4double r_val)
{
    fGr_val = r_val;
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......











void HistoManager::SetClusterPositionandMomentum(G4int evtNb, G4double x, G4double y, G4double Energy, G4double Px, G4double Py, G4double Pz)
{
    fEvtNb_ = evtNb;
    fClust_X_ = x;
    fClust_Y_ = y;
    fClust_Ene_ = Energy;
    fClust_Px_ = Px;
    fClust_Py_ = Py;
    fClust_Pz_ = Pz;
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void HistoManager::SetFluxEnergyandPID(G4int id, G4int PID, G4double ene)
{
    for(int i = 0 ; i < MaxNtuple ; i++){fFluxPID[i] = 0; fFluxEne[i] = 0.;}
    fFluxPID[id] = PID;
    fFluxEne[id] = ene;
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void HistoManager::SetFluxSphereEnergyandPID(G4int FluxPID, G4double energy, G4ThreeVector position)
{
    if(FluxPID == 22){
        fFluxEnergy_gamma = energy;
        fTheta_gamma = (180./pi)*(acos(position.z()/sqrt(position.x()*position.x() + position.y()*position.y() + position.z()*position.z())));
        fPhi_gamma = (180./pi)*(atan2(position.y(), position.x()));
    }
    else if(FluxPID == -11){
        fFluxEnergy_pos = energy;
        fTheta_pos = (180./pi)*(acos(position.z()/sqrt(position.x()*position.x() + position.y()*position.y() + position.z()*position.z())));
        fPhi_pos = (180./pi)*(atan2(position.y(), position.x()));
    }
    else if(FluxPID == 11){
        fFluxEnergy_elec = energy;
        fTheta_elec = (180./pi)*(acos(position.z()/sqrt(position.x()*position.x() + position.y()*position.y() + position.z()*position.z())));
        fPhi_elec = (180./pi)*(atan2(position.y(), position.x()));
    }
    else{
        fFluxEnergy_unknown = energy;
        fFluxPID_unknown = FluxPID;
        fTheta_unknown = (180./pi)*(acos(position.z()/sqrt(position.x()*position.x() + position.y()*position.y() + position.z()*position.z())));
        fPhi_unknown = (180./pi)*(atan2(position.y(), position.x()));
    }
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void HistoManager::SetFluxOptFiberEnergyandPID(G4int evtNb, G4int FluxPID, G4double energy, G4ThreeVector mom, G4ThreeVector position, G4ThreeVector localPosition)
{
    fFluxEvtNb = evtNb;
    if(FluxPID == 22){
        fFluxEnergy_OptFiber_gamma = energy;
        fFluxMomX_OptFiber_gamma = mom.x();
        fFluxMomY_OptFiber_gamma = mom.y();
        fFluxMomZ_OptFiber_gamma = mom.z();
        fGlobalX_OptFiber_gamma = position.x();
        fGlobalY_OptFiber_gamma = position.y();
        fGlobalZ_OptFiber_gamma = position.z();
        fLocalX_OptFiber_gamma = localPosition.x();
        fLocalY_OptFiber_gamma = localPosition.y();
        fLocalZ_OptFiber_gamma = localPosition.z();
    }
    else if(FluxPID == -11){
        fFluxEnergy_OptFiber_pos = energy;
        fFluxMomX_OptFiber_pos = mom.x();
        fFluxMomY_OptFiber_pos = mom.y();
        fFluxMomZ_OptFiber_pos = mom.z();
        fGlobalX_OptFiber_pos = position.x();
        fGlobalY_OptFiber_pos = position.y();
        fGlobalZ_OptFiber_pos = position.z();
        fLocalX_OptFiber_pos = localPosition.x();
        fLocalY_OptFiber_pos = localPosition.y();
        fLocalZ_OptFiber_pos = localPosition.z();
    }
    else if(FluxPID == 11){
        fFluxEnergy_OptFiber_elec = energy;
        fFluxMomX_OptFiber_elec = mom.x();
        fFluxMomY_OptFiber_elec = mom.y();
        fFluxMomZ_OptFiber_elec = mom.z();
        fGlobalX_OptFiber_elec = position.x();
        fGlobalY_OptFiber_elec = position.y();
        fGlobalZ_OptFiber_elec = position.z();
        fLocalX_OptFiber_elec = localPosition.x();
        fLocalY_OptFiber_elec = localPosition.y();
        fLocalZ_OptFiber_elec = localPosition.z();
    }
    else{
        fFluxEnergy_OptFiber_unknown = energy;
        fFluxMomX_OptFiber_unknown = mom.x();
        fFluxMomY_OptFiber_unknown = mom.y();
        fFluxMomZ_OptFiber_unknown = mom.z();
        fFluxPID_OptFiber_unknown = FluxPID;
        fGlobalX_OptFiber_unknown = position.x();
        fGlobalY_OptFiber_unknown = position.y();
        fGlobalZ_OptFiber_unknown = position.z();
        fLocalX_OptFiber_unknown = localPosition.x();
        fLocalY_OptFiber_unknown = localPosition.y();
        fLocalZ_OptFiber_unknown = localPosition.z();
    }
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
