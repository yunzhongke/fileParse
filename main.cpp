/*
 *
 * 功能：提取文件中有用的信息
 * 
 */

#include <utility>
#include <string>
#include <iostream>
#include <fstream>
#include <cstring>
#include <memory>
#include <vector>

#include <boost/filesystem.hpp>

struct ExtractFile
{
    ExtractFile(const char *jsonDir);
    void readFile();
    void transformPoint();
    void showJsonPoint();
    void addJsonPoint(const std::string &json , const std::vector<std::pair<double , double>> &point);
    std::vector<std::pair<double , double>>&& getPoint(const std::string &json);
    std::vector<std::string>&& getJson();
    void reset(); //重置

    std::vector<std::pair<std::string , std::vector<std::pair<double , double>>>> jsonPoint;
    std::vector<std::string> jsons; //存放json文件
    std::vector<std::pair<double , double>> Point;
    std::vector<double> coordinate; //保存坐标点
    int num; //计数器，遇到'['++ ,遇到']'--
    bool flag; //标记计数器开始累加，当计数器为0时，退出循环
};

void ExtractFile::reset()
{
    flag = false;
    num = 0;
    Point.clear();
    coordinate.clear(); 
}

ExtractFile::ExtractFile(const char *jsonDir)
{
    boost::filesystem::path pDir(jsonDir);
    for (boost::filesystem::directory_entry& element : boost::filesystem::directory_iterator(pDir)){
        if (boost::filesystem::is_regular_file(element.path()) && strstr(element.path().c_str() , ".json")){
            jsons.push_back(element.path().string());
        }
    }
    flag = false;
    num = 0;
}

std::vector<std::string>&& ExtractFile::getJson()
{
    return std::move(jsons);
}

std::vector<std::pair<double , double>>&& ExtractFile::getPoint(const std::string &json)
{
    for (std::vector<std::pair<std::string , std::vector<std::pair<double , double>>>>::const_iterator jsp = jsonPoint.begin();
            jsp != jsonPoint.end(); ++jsp ){
        if (strcmp(jsp->first.c_str() , json.c_str()) == 0){
            for (std::vector<std::pair<double , double>>::const_iterator it = jsp->second.begin(); it != jsp->second.end(); ++it){
                //std::cout << it->first << " , " << it->second << std::endl;
                Point.push_back(std::ref(*it));
            }
            break;
        }
    } 
    return std::move(Point);
}



void ExtractFile::transformPoint()
{
    bool tflag = true;
    double x , y;
    for (std::vector<double>::const_iterator it = coordinate.begin(); it != coordinate.end(); ++it){
        if (tflag){
            x = *it;
            tflag = !tflag;
        }
        else{
            y = *it;
            tflag = !tflag;

            Point.push_back(std::pair<double , double>(x , y));
        }
    }

}

void ExtractFile::showJsonPoint()
{

    for (std::vector<std::pair<std::string , std::vector<std::pair<double , double>>>>::const_iterator jsp = jsonPoint.begin();
            jsp != jsonPoint.end(); ++jsp ){
        std::cout << "当前文件为：" << jsp->first << std::endl;
        std::cout << "坐标点的信息如下：" << std::endl;
        for (std::vector<std::pair<double , double>>::const_iterator it = jsp->second.begin(); it != jsp->second.end(); ++it){
            std::cout << it->first << " , " << it->second << std::endl;
        }
        std::cout << "\n";
    }
}

void ExtractFile::readFile()
{
    for (std::vector<std::string>::const_iterator it = jsons.begin(); it != jsons.end(); ++it){
        char buf[256];
        std::ifstream ifstr(*it);
        while(!ifstr.eof()){
            memset(buf , 0 , sizeof(buf) / sizeof(buf[0]));
            ifstr.getline(buf , 256 , '\n');

            if (flag){
                if (!num){ // 将flag重置false , 因为一个文件中可能有多个"polygon"
                    flag = false;
                }
                else{
                    if (strchr(buf , '['))
                        ++num;
                    else if (strchr(buf , ']'))
                        --num;
                    else {
                        coordinate.push_back(std::atof(buf)); 
                    }
                }
            }
            else{
                if (strstr(buf , "polygon")){
                    flag = true; //设置标志
                    if (strchr(buf , '['))
                        ++num;
                }
            }

        }
        transformPoint();
        addJsonPoint(*it , Point);
        //showPoint();
        ifstr.close();
        reset();
    }
}

void ExtractFile::addJsonPoint(const std::string &json , const std::vector<std::pair<double , double>> &point)
{
    jsonPoint.push_back(std::pair<std::string , std::vector<std::pair<double , double>>>(json , point));
}


int main(int argc , char *argv[])
{
    if (argc !=2 ){

        std::cerr << "usage fileParse jsonpath" << "\n";
        return -1;
    }
    try{
        ExtractFile file(argv[1]);
        file.readFile();
        file.showJsonPoint();
    }
    catch (std::exception &e){
        std::cout << "Error: " << e.what() << std::endl;
    }

    return 0;
}

