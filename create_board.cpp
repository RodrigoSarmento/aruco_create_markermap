/*****************************
Copyright 2016 Rafael Muñoz Salinas. All rights reserved.

Redistribution and use in source and binary forms, with or without modification, are
permitted provided that the following conditions are met:

   1. Redistributions of source code must retain the above copyright notice, this list of
      conditions and the following disclaimer.

   2. Redistributions in binary form must reproduce the above copyright notice, this list
      of conditions and the following disclaimer in the documentation and/or other materials
      provided with the distribution.

THIS SOFTWARE IS PROVIDED BY Rafael Muñoz Salinas ''AS IS'' AND ANY EXPRESS OR IMPLIED
WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND
FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL Rafael Muñoz Salinas OR
CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

The views and conclusions contained in the software and documentation are those of the
authors and should not be interpreted as representing official policies, either expressed
or implied, of Rafael Muñoz Salinas.
********************************/


//Creation of MarkerMap to be printed in a piece of paper. This is the old boards in Aruco 1.x.x
#include <algorithm>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "markermap.h"
#include <cstdio>
#include "dictionary.h"
#include <opencv2/highgui/highgui.hpp>
#include <fstream>

int ids_usados[250];

class CmdLineParser{int argc; char **argv; public: CmdLineParser(int _argc,char **_argv):argc(_argc),argv(_argv){}  bool operator[] ( string param ) {int idx=-1;  for ( int i=0; i<argc && idx==-1; i++ ) if ( string ( argv[i] ) ==param ) idx=i;    return ( idx!=-1 ) ;    } string operator()(string param,string defvalue="-1"){int idx=-1;    for ( int i=0; i<argc && idx==-1; i++ ) if ( string ( argv[i] ) ==param ) idx=i; if ( idx==-1 ) return defvalue;   else  return ( argv[  idx+1] ); }};
void checar_ids(int ids_usados[], int k);

using namespace std;
using namespace cv;

int main(int argc, char **argv) {
        CmdLineParser cml(argc,argv);

        int XSize,YSize,maximo,k=0;
;
        cout<<"How many boards?,EX: 4 \n";
        cin>>maximo;
        cout<<"number of raws and columns,EX: 2 2\n";
        cin>>XSize>>YSize;

        auto Dict=aruco::Dictionary::loadPredefined(cml("-d","ARUCO_MIP_36h12"));
        int typeMarkerMap = stoi(cml("-t","0"));

        srand(time(0));
        //vetor para checagem de ids usados
        for(int i=0;i<250;i++){
        ids_usados[i]=-1;
        }
        //arquivo onde deve ser salvos as ids
        ofstream arq;
        arq.open("lista_IDS.txt");
        //loop para salvarmos varios boards unicos
        for(int i=0;i<maximo;i++){

        vector<int> ids;
        //nova id aleatoria
        arq<<"Board n° "<<i<<": ";
        for(int j=0;j<XSize*YSize;j++,k++){
            ids_usados[k]=rand()%250;
            checar_ids(ids_usados,k);
            ids.push_back(ids_usados[k]);
        }

        ids.resize(XSize*YSize);


        //take the required ones
        aruco::MarkerMap BInfo=Dict.createMarkerMap(Size(XSize, YSize), 500, 500 * 0.2,ids,typeMarkerMap==1);
        //create a printable image to save
        cv::Mat MarkerMapImage=BInfo.getImage();

        //save
        string salvar_yml="map"+to_string(i)+".yml";
        string salvar_png="map"+to_string(i)+".png";
        BInfo.saveToFile(salvar_yml);
        imwrite(salvar_png, MarkerMapImage);
        }
    for(int j=0;j<100;j++){
        cout<<ids_usados[j]<<endl;
        }



}
void checar_ids(int ids_usados[], int k){
    for(int j=0;j<250;j++){
            if(k==j) continue;
            if(ids_usados[k]==ids_usados[j]){
                do{
                    ids_usados[k]=rand()%250;
                }while(ids_usados[k]==ids_usados[j]);
            }
        }

}
