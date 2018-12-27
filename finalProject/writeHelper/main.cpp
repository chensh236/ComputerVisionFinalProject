#include <iostream>
#include <cstring>
#include <fstream>
#include <vector>
#include <io.h>
#include <direct.h>
using namespace std;

void beginProcess(){
    string resultPath = "../../A4Warping/result/result.csv";
    ofstream ofs(resultPath.c_str());
    ofs<<"filename"<<","<<"\"corner 1\""<<","<<"\"corner 2\""<<","<<"\"corner 3\""<<","<<"\"corner 4\""<<","<<"\"student ID\""<<","<<"\"phone number\""<<","<<"\"ID card\""<<endl;

    for(int i = 0; i < 100; i ++){
        string subDir = "../../A4Warping/temp/img" + to_string(i);
        string pointsPath = subDir + "/points.txt";
        string detectionPath = subDir + "/result.txt";
        ifstream ifs1(pointsPath.c_str());
        string file = to_string(i + 1) + ".bmp";
        ofs<<file<<",";
        vector< vector<string> > pointVec;
        // points
        string strLine;
        while(getline(ifs1, strLine))
        {
            int mid = 0, end = 0;
            while(strLine[mid] != ' ' && mid < strLine.length()){
                mid++;
            }

            string sub1 = "", sub2 = "";
            for(int index = 0; index < mid; index++){
                sub1 += strLine[index];
            }
            for(int index = mid + 1; index < strLine.length(); index++){
                sub2 += strLine[index];
            }
            string stich = "\"";
            stich += sub1;
            stich += ",";
            stich += sub2;
            stich +="\",";
            //cout<<stich<<endl;
            ofs<<stich;
            vector<string> tmp;
            tmp.push_back(sub1);
            tmp.push_back(sub2);
            pointVec.push_back(tmp);
        }
        ifs1.close();

        ifstream ifs2(detectionPath.c_str());
        // result
        int line = 0;
        bool isFirst = true;
        while(getline(ifs2, strLine))
        {

            //cout<<strLine<<endl;

            //重复
            if(!isFirst && line == 0){
                ofs<<file<<",";
                for(int j = 0; j < 4; j++){
                    string stich = "\"";
                    stich += pointVec[j][0];
                    stich += ",";
                    stich += pointVec[j][1];
                    stich +="\",";
                    ofs<<stich;
                }
                string stich = strLine;
                stich += ",";
                ofs<<stich;
            } else{
                if(line == 2){
                    ofs<<strLine<<"\t"<<endl;
                }else{
                    ofs<<strLine<<"\t,";
                }
            }
            line++;
            line %= 3;
            if(isFirst){
                isFirst = false;
            }
        }
        ifs1.close();
    }
    ofs.close();
}


int main() {

    string dir="../../A4Warping/result";
    if (access(dir.c_str(), 0) == -1)
    {
        cout<<dir<<" is not existing"<<endl;
        cout<<"now make it"<<endl;
#ifdef WIN32
        int flag=mkdir(dir.c_str());
#endif
#ifdef linux
        int flag=mkdir(dir.c_str(), 0777);
#endif
        if (flag == 0)
        {
            cout<<"make successfully"<<endl;
        } else {
            cout<<"make errorly"<<endl;
        }
    }
    if(access(dir.c_str(), 0) == 0){
        beginProcess();
    } else{
        cout<<"Fail!"<<endl;
        exit(0);
    }
    return 0;
}