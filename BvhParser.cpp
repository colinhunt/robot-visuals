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

void setChannelsOrder(Joint &joint, stringstream &lineStream);

void setMap(int* map, stringstream &lineStream);

void exitWithBadBvh() {
    cerr << "Bad bvh file" << endl;
    exit(EXIT_FAILURE);
}

void verifyLine(ifstream& myfile, string& line, string text) {
    getline(myfile, line);
    if (line.substr(0, text.length()) != text) {
        exitWithBadBvh();
    }
}



void dataFromBVH(ifstream &myfile, BvhData &data) {
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
    parseHierarchy(myfile, data.skeleton.root, data.motion.rotationsPerFrame);

//    parseMotion(myfile, data.motion, data.skeleton.root);
}

void parseMotion(ifstream &myfile, Motion &motion, const Joint& root) {
    string line;
    while (getline(myfile, line))
        if (line.substr(0,6) != "MOTION")
            continue;

    string token;
    stringstream lineStream(line);
    lineStream >> token;
    if (token != "Frames:")
        exitWithBadBvh();

    lineStream >> motion.numFrames;
    lineStream >> token >> token >> motion.frameTime;

    while (getline(myfile, line)) {
        Frame f;
        stringstream lineStream(line);
        double u, v, w, x, y, z;
        lineStream >> u >> v >> w;
        f.translation = root.getTranslation(u, v, w);
        for (int i = 3; i < motion.rotationsPerFrame; i += 3) {
            lineStream >> w >> v >> u;
            Quaterniond q;
            q = AngleAxisd(w, Vector3d::UnitZ())
                    * AngleAxisd(v, Vector3d::UnitY())
                    * AngleAxisd(u, Vector3d::UnitX());
            f.rotations.push_back(q);
        }
    }
}

void parseHierarchy(ifstream &myfile, Joint &joint, int &id) {
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
            parseHierarchy(myfile, child, id);
            joint.children.push_back(child);
        } else if (token == "End") {
            child.id = -1;
            child.name = "End Site";
            cout << "Adding child... " << child.name << endl;
            parseHierarchy(myfile, child, id);
            joint.children.push_back(child);
        } else if (token == "OFFSET") {
            double x, y, z;
            lineStream >> x >> y >> z;
            cout << "OFFSET " << x << " " << y << " " << z << endl;
            joint.offset = Vector3d(x, y, z);
        } else if (token == "CHANNELS") {
            setChannelsOrder(joint, lineStream);
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

void setChannelsOrder(Joint &joint, stringstream &lineStream) {
    int num;
    lineStream >> num;
    cout << "Setting channels order " << num << endl;

    if (num == 6) {
        setMap(joint.positionMap, lineStream);
        cout << "Pmap: " << joint.positionMap[0]
                << " " << joint.positionMap[1]
                << " " << joint.positionMap[2] << endl;
    }
    setMap(joint.rotationMap, lineStream);
    cout << "Rmap: " << joint.rotationMap[0]
            << " " << joint.rotationMap[1]
            << " " << joint.rotationMap[2] << endl;
}

void setMap(int* map, stringstream &lineStream) {
    string c[3];
    lineStream >> c[0] >> c[1] >> c[2];
    for (int i = 0; i < 3; ++i) {
        if (c[i][0] == 'X')
            map[0] = i;
        else if (c[i][0] == 'Y')
            map[1] = i;
        else if (c[i][0] == 'Z')
            map[2] = i;
    }
}

vector<Frame> framesFromBVH(char* filename) {
    vector<Frame> frames;



    return frames;
}

void parseBvhFile(char* fileName, BvhData &data) {
    string line;
    ifstream myfile(fileName);
    if (myfile.is_open()) {
        dataFromBVH(myfile, data);
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