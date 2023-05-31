void analyze()
{
  TFile fInput("RAONSCL2.root", "READ");

  Int_t     event_type;
  Char_t    name[32];
  Double_t  kineticEnergy;
  Double_t  totalEnergy;
  Double_t  vertex_x, vertex_y, vertex_z;
  Double_t  px, py, pz;
  Double_t  theta, phi;
  ULong64_t nEntries;

  TH1D* hKineticEnergy = new TH1D("hKineticEnergy", ";Kinetic Energy (MeV);Entries", 120, 0, 600);
  TH1D* hTotalEnergy  = new TH1D("hTotalEnergy",  ";Total Energy (MeV);Entries", 308, 0, 1540); // 1540 = 600 + 940, 940 ~ 939 MeV = M_n, neutron mass
  TH1D* hvx = new TH1D("hvx", ";Vertex Position, #it{x} (cm);Entries", 120, -60, 60);
  TH1D* hvy = new TH1D("hvy", ";Vertex Position, #it{y} (cm);Entries", 120, -60, 60);
  TH1D* hvz = new TH1D("hvz", ";Vertex Position, #it{z} (cm);Entries", 120, -60, 60);
  TH2D* hvx_vy = new TH2D("hvx_vy", ";Vertex Position, #it{x} (cm);Vertex Position, #it{y} (cm)", 120, -60, 60, 120, -60, 60);
  TH2D* hvz_vy = new TH2D("hvy_vz", ";Vertex Position, #it{z} (cm);Vertex Position, #it{y} (cm)", 120, -60, 60, 120, -60, 60);
  TH1D* hTheta = new TH1D("hTheta", ";Angle w.r.to z-axis, #theta (deg.);Entries/0.1 (deg)", 3600, 0, 360);
  TH1D* hPhi  = new TH1D("hPhi", ";Azimuthal angle, #phi (deg.);Entries/0.1 (deg)", 3600, -180, 180);

  TTree* fTree = (TTree*)fInput.Get("RAONSCL2");

  fTree->SetBranchAddress("name", name);
  fTree->SetBranchAddress("kineticEnergy", &kineticEnergy);
  fTree->SetBranchAddress("totalEnergy", &totalEnergy);
  fTree->SetBranchAddress("vertex_x", &vertex_x);
  fTree->SetBranchAddress("vertex_y", &vertex_y);
  fTree->SetBranchAddress("vertex_z", &vertex_z);
  fTree->SetBranchAddress("px", &px);
  fTree->SetBranchAddress("py", &py);
  fTree->SetBranchAddress("pz", &pz);

  nEntries = fTree->GetEntries();


  for(ULong64_t i = 0; i < nEntries; i++)
  {
    fTree->GetEntry(i);
    theta = acos( pz / sqrt( px*px + py*py + pz*pz ) ) * TMath::RadToDeg();
    phi   = atan2( py, pz ) * TMath::RadToDeg();
    if( strcmp("neutron", name) == 0 )
    {
      hKineticEnergy->Fill(kineticEnergy);
      hTotalEnergy->Fill(totalEnergy);
      hvx->Fill(vertex_x);
      hvy->Fill(vertex_y);
      hvz->Fill(vertex_z);
      hvx_vy->Fill(vertex_x, vertex_y);
      hvz_vy->Fill(vertex_z, vertex_y);
      hTheta->Fill(theta);
      hPhi->Fill(phi);
    }
  }

  TFile* fOutput = new TFile("output_analysis.root", "recreate");
  hKineticEnergy->Write();
  hTotalEnergy->Write();
  hvx->Write();
  hvy->Write();
  hvz->Write();
  hvx_vy->Write();
  hvz_vy->Write();
  hTheta->Write();
  hPhi->Write();
  fOutput->Close();
  fInput.Close();
}
