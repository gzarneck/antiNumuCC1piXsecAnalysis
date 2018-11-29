#include "antiNumuCC1piXsecAnalysis.hxx"
#include "AnalysisLoop.hxx"

int main(int argc, char *argv[]){
  antiNumuCC1piXsecAnalysis* ana = new antiNumuCC1piXsecAnalysis();
  AnalysisLoop loop(ana, argc, argv); 
  loop.Execute();
}
