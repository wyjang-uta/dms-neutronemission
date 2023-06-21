// This script extract values in ROOT file and save it as txt format.
//
// Column structure:
//    <x> (cm)  <y> (cm)  <z> (cm)    <px> (MeV)   <py> (MeV)   <pz> (MeV)   <kinetic energy> (MeV)
// usage:
// $ root -l 'root2txt.C("<input file name>", "<output txt file name>")'
//

void root2txt(const char* input = "DMSNeutronEmission.root", const char* output = "DMSNeutronEmission.txt")
{
  Double_t kineticEnergy;
  Double_t hit_x, hit_y, hit_z;
  Double_t px, py, pz;
  Double_t globalTime;

  TFile* ifile = new TFile(input, "read");
  TTree* tree = (TTree*)ifile->Get("RAONSCL2");
  tree->SetBranchAddress("kineticEnergy", &kineticEnergy); // unit in MeV
  tree->SetBranchAddress("hit_x", &hit_x);                 // unit in cm
  tree->SetBranchAddress("hit_y", &hit_y);
  tree->SetBranchAddress("hit_z", &hit_z);
  tree->SetBranchAddress("px", &px);                       // unit in MeV
  tree->SetBranchAddress("py", &py);
  tree->SetBranchAddress("pz", &pz);

  ULong64_t nEntries = tree->GetEntries();

  ofstream out(output);

  cout << "Loop over " << nEntries << " events." << endl;
  for( ULong64_t i = 0; i < nEntries; ++i )
  {
    tree->GetEntry(i);
    out << hit_x << " " << hit_y << " " << hit_z << " " << px << " " << py << " " << pz << " " << kineticEnergy << endl;
  }

  out.close();
  ifile->Close();
}
