#include "BvhParser.h"

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <sys/errno.h>

void parseHierarchy(ifstream &myfile, Joint &joint, int &id);
void dataFromBVH(ifstream &myfile, BvhData &data);
vector<Frame> framesFromBVH(char* filename);
void parseMotion(ifstream &myfile, Motion &motion, Joint& root);

class BvhParser {
public:
    BvhParser(ifstream& myfile, BvhData& data) : myfile(myfile), data(data) {

    }

    void parse() {
        string line;
        verifyLine(myfile, line, "HIERARCHY");
        verifyLine(myfile, line, "ROOT");

        string token;
        stringstream lineStream(line);
        lineStream >> token;
        cout << "Token is: " << token << endl;
        if (token != "ROOT") {
            exitWithBadBvh();
        }

        data.motion.rotationsPerFrame = 0;
        data.skeleton.root.id = data.motion.rotationsPerFrame++;
        lineStream >> data.skeleton.root.name;
        cout << "Skeleton name is " << data.skeleton.root.name << endl;

        parseHierarchy(data.skeleton.root, data.motion.rotationsPerFrame);
//        parseMotion();
    }

private:
    void verifyLine(ifstream& myfile, string& line, string text) {
        getline(myfile, line);
        if (line.substr(0, text.length()) != text) {
            exitWithBadBvh();
        }
    }
    void exitWithBadBvh() {
        cerr << "Bad bvh file" << endl;
        exit(EXIT_FAILURE);
    }

    void parseMotion() {
        string line;
        while (getline(myfile, line))
            if (line.substr(0,6) != "MOTION")
                continue;

        string token;
        stringstream lineStream(line);
        lineStream >> token;
        if (token != "Frames:")
            exitWithBadBvh();

        lineStream >> data.motion.numFrames;
        lineStream >> token >> token >> data.motion.frameTime;

        while (getline(myfile, line)) {
            Frame f;
            stringstream lineStream(line);
            double u, v, w, x, y, z;
            lineStream >> u >> v >> w;
            for (int i = 3; i < data.motion.rotationsPerFrame; i += 3) {
                lineStream >> w >> v >> u;
                Quaterniond q;
                q = AngleAxisd(w, Vector3d::UnitZ())
                        * AngleAxisd(v, Vector3d::UnitY())
                        * AngleAxisd(u, Vector3d::UnitX());
                f.rotations.push_back(q);
            }
        }
    }

    void parseHierarchy(Joint &joint, int& id) {
        string line;
        while (getline(myfile, line)) {
            string token;
            stringstream lineStream(line);
            lineStream >> token;
            cout << "Token is: " << token << endl;
            Joint child;
            if (token == "JOINT") {
                child.id = id++;
                lineStream >> child.name;
                cout << "Adding child... " << child.name << endl;
                parseHierarchy(child, id);
                joint.children.push_back(child);
            } else if (token == "End") {
                child.id = -1;
                child.name = "End Site";
                cout << "Adding child... " << child.name << endl;
                parseHierarchy(child, id);
                joint.children.push_back(child);
            } else if (token == "OFFSET") {
                double x, y, z;
                lineStream >> x >> y >> z;
                cout << "OFFSET " << x << " " << y << " " << z << endl;
                joint.offset = Vector3d(x, y, z);
            } else if (token == "CHANNELS") {
                int num;
                lineStream >> num;
                for (int i = 0; i < num; ++i) {
                    string channel;
                    lineStream >> channel;
                    data.motion.channels.push_back(channel);
                }
            } else if (token == "{") {
                // skip
            } else if (token == "}") {
                break; // stop parsing this joint
            } else {
                cerr << "Unrecognized token " << token << endl;
                exit(EXIT_FAILURE);
            }
        }
    }


    BvhData& data;
    ifstream& myfile;
};


//void exitWithBadBvh() {
//    cerr << "Bad bvh file" << endl;
//    exit(EXIT_FAILURE);
//}
//
//void verifyLine(ifstream& myfile, string& line, string text) {
//    getline(myfile, line);
//    if (line.substr(0, text.length()) != text) {
//        exitWithBadBvh();
//    }
//}
//
//
//
//void dataFromBVH(ifstream &myfile, BvhData& data) {
//    string line;
//    verifyLine(myfile, line, "HIERARCHY");
//    verifyLine(myfile, line, "ROOT");
//
//    string token;
//    stringstream lineStream(line);
//    lineStream >> token;
//    cout << "Token is: " << token << endl;
//    if (token != "ROOT") {
//        exitWithBadBvh();
//    }
//
//    data.motion.rotationsPerFrame = 0;
//    data.skeleton.root.id = data.motion.rotationsPerFrame++;
//    lineStream >> data.skeleton.root.name;
//    cout << "Skeleton name is " << data.skeleton.root.name << endl;
//    parseHierarchy(myfile, data);
//
//    parseMotion(myfile, data);
//}
//
//void parseMotion(ifstream &myfile, Motion &motion, const Joint& root) {
//    string line;
//    while (getline(myfile, line))
//        if (line.substr(0,6) != "MOTION")
//            continue;
//
//    string token;
//    stringstream lineStream(line);
//    lineStream >> token;
//    if (token != "Frames:")
//        exitWithBadBvh();
//
//    lineStream >> motion.numFrames;
//    lineStream >> token >> token >> motion.frameTime;
//
//    while (getline(myfile, line)) {
//        Frame f;
//        stringstream lineStream(line);
//        double u, v, w, x, y, z;
//        lineStream >> u >> v >> w;
//        for (int i = 3; i < motion.rotationsPerFrame; i += 3) {
//            lineStream >> w >> v >> u;
//            Quaterniond q;
//            q = AngleAxisd(w, Vector3d::UnitZ())
//                    * AngleAxisd(v, Vector3d::UnitY())
//                    * AngleAxisd(u, Vector3d::UnitX());
//            f.rotations.push_back(q);
//        }
//    }
//}
//
//void parseHierarchy(ifstream &myfile, Joint &joint, int &id) {
//    string line;
//    while (getline(myfile, line)) {
//        string token;
//        stringstream lineStream(line);
//        lineStream >> token;
//        cout << "Token is: " << token << endl;
//        Joint child;
//        if (token == "JOINT") {
//            child.id = id++;
//            lineStream >> child.name;
//            cout << "Adding child... " << child.name << endl;
//            parseHierarchy(myfile, child, id);
//            joint.children.push_back(child);
//        } else if (token == "End") {
//            child.id = -1;
//            child.name = "End Site";
//            cout << "Adding child... " << child.name << endl;
//            parseHierarchy(myfile, child, id);
//            joint.children.push_back(child);
//        } else if (token == "OFFSET") {
//            double x, y, z;
//            lineStream >> x >> y >> z;
//            cout << "OFFSET " << x << " " << y << " " << z << endl;
//            joint.offset = Vector3d(x, y, z);
//        } else if (token == "CHANNELS") {
//            int num;
//            string p[3];
//            lineStream >> num;
//            for (int i = 0; i < num; ++i) {
//                string channel;
//                lineStream >> channel;
//
//            }
//        } else if (token == "{") {
//            // skip
//        } else if (token == "}") {
//            break; // stop parsing this joint
//        } else {
//            cerr << "Unrecognized token " << token << endl;
//            exit(EXIT_FAILURE);
//        }
//    }
//}
//
//vector<Frame> framesFromBVH(char* filename) {
//    vector<Frame> frames;
//
//
//
//    return frames;
//}

void parseBvhFile(char* fileName, BvhData &data) {
    string line;
    ifstream myfile(fileName);
    if (myfile.is_open()) {
//        dataFromBVH(myfile, data);
        BvhParser parser(myfile, data);
        parser.parse();
    } else {
        if (myfile.fail())
            cout << "Logical error" << endl;
        if (myfile.bad())
            cout << "Read/writing error" << endl;
        cout << "Unable to open file" << endl;
        cerr << "Error: " << strerror(errno) << endl;
        exit(errno);
    }
}