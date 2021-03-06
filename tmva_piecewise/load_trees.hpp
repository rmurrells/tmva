

#include "TFile.h"
#include "TTree.h"


struct load_trees {


  TFile * ifile_sp;
  TFile * ifile_bnb;
  TFile * ifile_bnb_cosmic;
  TFile * ifile_cosmic;
  TFile * ifile_sp_cosmic;
  
  TTree * vertex_tree_sp;
  TTree * vertex_tree_bnb;
  TTree * vertex_tree_bnb_cosmic;
  TTree * vertex_tree_cosmic;
  TTree * vertex_tree_sp_cosmic;
  
  TTree * event_tree_sp;
  TTree * event_tree_bnb;
  TTree * event_tree_bnb_cosmic;
  TTree * event_tree_cosmic;
  TTree * event_tree_sp_cosmic;
  
  double pot_sp = -1;
  double pot_sp_cosmic = -1;
  double pot_bnb = -1;
  double pot_bnb_cosmic = -1;
  double ngenbnbcosmic = -1;
  double ngencosmic = -1;
  
  std::string const signal_definition;

  load_trees(std::string dir, std::string const & friend_tree_file = "") :
    signal_definition("is_delta_rad == 1 && true_nu_vtx_fid_contained == 1") {
    
    ifile_sp = nullptr;
    ifile_bnb = nullptr;
    ifile_bnb_cosmic = nullptr;
    ifile_cosmic = nullptr;
    ifile_sp_cosmic = nullptr;
    
    vertex_tree_sp = nullptr;
    vertex_tree_bnb = nullptr;
    vertex_tree_bnb_cosmic = nullptr;
    vertex_tree_cosmic = nullptr;
    vertex_tree_sp_cosmic = nullptr;
    
    event_tree_sp = nullptr;
    event_tree_bnb = nullptr;
    event_tree_bnb_cosmic = nullptr;
    event_tree_cosmic = nullptr;
    event_tree_sp_cosmic = nullptr;
    
    pot_sp = -1;
    pot_sp_cosmic = -1;
    pot_bnb = -1;
    pot_bnb_cosmic = -1;
    ngenbnbcosmic = -1;
    ngencosmic = -1;

    init(dir, friend_tree_file);
    
  }
  
  std::vector<std::string> friend_tree_names;

  void init(std::string dir, std::string const & friend_tree_file = "") {
    
    if(dir != "") dir += "/";
    std::string gdir;
    if(dir.find("old") == std::string::npos) gdir = "LEEPhoton/";
    
    ifile_sp = TFile::Open((dir+"runmv_sp.root").c_str());
    if(ifile_sp) {
      event_tree_sp = (TTree*)ifile_sp->Get((gdir+"event_tree").c_str());
      vertex_tree_sp = (TTree*)ifile_sp->Get((gdir+"vertex_tree").c_str());  
      TTree * pot_tree_sp = (TTree*)ifile_sp->Get((gdir+"get_pot").c_str());
      pot_tree_sp->SetBranchAddress("pot", &pot_sp);
      pot_tree_sp->GetEntry(0);
    }    
    
    ifile_bnb = TFile::Open((dir+"runmv_bnb.root").c_str());
    if(ifile_bnb) {
      event_tree_bnb = (TTree*)ifile_bnb->Get((gdir+"event_tree").c_str());
      vertex_tree_bnb = (TTree*)ifile_bnb->Get((gdir+"vertex_tree").c_str());
      TTree * pot_tree_bnb = (TTree*)ifile_bnb->Get((gdir+"get_pot").c_str());
      pot_tree_bnb->SetBranchAddress("pot", &pot_bnb);
      pot_tree_bnb->GetEntry(0);
    }
    
    ifile_bnb_cosmic = TFile::Open((dir+"runmv_bnb_cosmic.root").c_str());
    if(ifile_bnb_cosmic) {
      event_tree_bnb_cosmic = (TTree*)ifile_bnb_cosmic->Get((gdir+"event_tree").c_str());
      vertex_tree_bnb_cosmic = (TTree*)ifile_bnb_cosmic->Get((gdir+"vertex_tree").c_str());
      TTree * pot_tree_bnb_cosmic = (TTree*)ifile_bnb_cosmic->Get((gdir+"get_pot").c_str());
      pot_tree_bnb_cosmic->SetBranchAddress("pot", &pot_bnb_cosmic);
      int temp_ngenbnbcosmic;
      pot_tree_bnb_cosmic->SetBranchAddress("number_of_events", &temp_ngenbnbcosmic);
      pot_tree_bnb_cosmic->GetEntry(0);
      ngenbnbcosmic = temp_ngenbnbcosmic;
    }
    
    ifile_cosmic = TFile::Open((dir+"runmv_cosmic.root").c_str());
    if(ifile_cosmic) {
      event_tree_cosmic = (TTree*)ifile_cosmic->Get((gdir+"event_tree").c_str());
      vertex_tree_cosmic = (TTree*)ifile_cosmic->Get((gdir+"vertex_tree").c_str());
      TTree * pot_tree_cosmic = (TTree*)ifile_cosmic->Get((gdir+"get_pot").c_str());
      int temp_ngencosmic;
      pot_tree_cosmic->SetBranchAddress("number_of_events", &temp_ngencosmic);
      pot_tree_cosmic->GetEntry(0);
      ngencosmic = temp_ngencosmic;
    }
    
    ifile_sp_cosmic = TFile::Open((dir+"runmv_sp_cosmic.root").c_str());
    if(ifile_sp_cosmic) {
      event_tree_sp_cosmic = (TTree*)ifile_sp_cosmic->Get((gdir+"event_tree").c_str());
      vertex_tree_sp_cosmic = (TTree*)ifile_sp_cosmic->Get((gdir+"vertex_tree").c_str());  
      TTree * pot_tree_sp_cosmic = (TTree*)ifile_sp_cosmic->Get((gdir+"get_pot").c_str());
      pot_tree_sp_cosmic->SetBranchAddress("pot", &pot_sp_cosmic);
      pot_tree_sp_cosmic->GetEntry(0);
    }
    
    if(friend_tree_file != "") {
      std::vector<std::string> const method_v = {
	"BDTG",
	"BDT",
	"BDTB",
	"BDTD",
	"BDTF",
	"RuleFit"
      };
      for(std::string const & method : method_v) {
	friend_tree_names.push_back(std::string(vertex_tree_sp->AddFriend(("ncdelta_"+method).c_str(), friend_tree_file.c_str())->GetTree()->GetName()));
	friend_tree_names.push_back(std::string(vertex_tree_sp_cosmic->AddFriend(("ncdelta_cosmic_"+method).c_str(), friend_tree_file.c_str())->GetTree()->GetName()));
	friend_tree_names.push_back(std::string(vertex_tree_bnb_cosmic->AddFriend(("bnb_cosmic_"+method).c_str(), friend_tree_file.c_str())->GetTree()->GetName()));
      }
    }
    
  }


};

