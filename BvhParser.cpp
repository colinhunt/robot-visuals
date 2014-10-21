#include "BvhParser.h"

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <sys/errno.h>
#include <Python/Python.h>


class BvhParser {

public:
    BvhParser(ifstream &myfile, BvhData &data) : myfile(myfile), data(data) {

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
        parseMotion();
        calculateSkelBox(data.skeleton.root);
        // make it a proper cube to capture skeleton range of motion inside
        // l, w, h of box
        Vector3d span = data.skeleton.maxP - data.skeleton.minP;
        data.skeleton.maxP[2] = span.x() / 2;
        data.skeleton.minP[2] = -(span.x() / 2);
    }

private:
    void verifyLine(ifstream &myfile, string &line, string text) {
        getline(myfile, line);
        if (line.substr(0, text.length()) != text) {
            exitWithBadBvh();
        }
    }

    void exitWithBadBvh() {
        cerr << "Bad bvh file" << endl;
        exit(EXIT_FAILURE);
    }

    Vector3d getTranslation(int i, double n);

    AngleAxisd getQuaternion(int i, double n);

    void parseMotion() {
        string line;
        while (getline(myfile, line)) {
            if (line.substr(0, 6) == "MOTION")
                break;
        }
        getline(myfile, line);
        string token;
        stringstream lineStream(line);
        lineStream >> token;
        cout << "Token is: " << token << endl;
        if (token != "Frames:")
            exitWithBadBvh();

        lineStream >> data.motion.numFrames;
        getline(myfile, line);
        stringstream lineStream2(line);
        lineStream2 >> token >> token >> data.motion.frameTime;
        cout << "Frames: " << data.motion.numFrames << endl;
        cout << "Frame Time: " << data.motion.frameTime << endl;
        cout << "Rotations/Frame: " << data.motion.rotationsPerFrame << endl;
        while (getline(myfile, line)) {
            Frame f;
            stringstream lineStream(line);
            double n1, n2, n3;
            lineStream >> n1 >> n2 >> n3;
            f.translation = getTranslation(0, n1)
                    + getTranslation(1, n2)
                    + getTranslation(2, n3);
            GltUtil::setMax(data.motion.maxP, f.translation);
            GltUtil::setMin(data.motion.minP, f.translation);
            cout << "min so far: " << data.motion.minP << endl;
            for (int i = 0; i < data.motion.rotationsPerFrame; ++i) {
                int fi = (i + 1) * 3;
                lineStream >> n1 >> n2 >> n3;
                Quaterniond q;
                q = getQuaternion(fi, n1)
                        * getQuaternion(fi + 1, n2)
                        * getQuaternion(fi + 2, n3);
                f.rotations.push_back(q);
            }
            data.motion.frames.push_back(f);
        }
    }

    void addChild(Joint& child, Joint& parent, int& id) {
        cout << "Adding child... " << child.name << endl;
        parseHierarchy(child, id);
        parent.children.push_back(child);
    }


    void parseHierarchy(Joint &joint, int &id) {
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
                addChild(child, joint, id);
            } else if (token == "End") {
                child.id = -1;
                child.name = "End Site";
                addChild(child, joint, id);
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

    void calculateSkelBox(Joint& joint) {
        for (int i = 0; i < joint.children.size(); ++i) {
            Joint& child = joint.children[i];
            child.absPosition += joint.absPosition + child.offset;
            cout << "Child abs: \n" << child.absPosition << endl;
            cout << "= Parent abs: \n" << joint.absPosition << endl;
            cout << "+ Child offset: \n" << child.offset << endl;
            GltUtil::setMax(data.skeleton.maxP, child.absPosition);
            GltUtil::setMin(data.skeleton.minP, child.absPosition);
            calculateSkelBox(child);
        }
    }

    BvhData &data;
    ifstream &myfile;
};


void parseBvhFile(char* fileName, BvhData &data) {
    string line;
    ifstream myfile(fileName);
    if (myfile.is_open()) {
        BvhParser parser(myfile, data);
        parser.parse();
        cout << data.motion.channels.size() << endl;
        cout << data.motion.frames.size() << endl;
        cout << data.motion.frames[0].rotations.size() << endl;
        for (int i = 0; i < data.motion.frames[0].rotations.size(); ++i) {
            Quaterniond q = data.motion.frames[0].rotations[i];
            cout << q.w() << q.x() << q.y() << q.z() << ":";
        }
        cout << endl;
        cout << "Motion max point: " << data.motion.maxP << endl;
        cout << "Motion min point: " << data.motion.minP << endl;
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

Vector3d BvhParser::getTranslation(int i, double n) {
    if (data.motion.channels[i] == "Xposition")
        return Vector3d(n, 0, 0);
    if (data.motion.channels[i] == "Yposition")
        return Vector3d(0, n, 0);
    if (data.motion.channels[i] == "Zposition")
        return Vector3d(0, 0, n);

    cerr << "Unrecognized translation\n";
    exit(EXIT_FAILURE);
}

AngleAxisd BvhParser::getQuaternion(int i, double n) {
    n = GltUtil::toRadians(n);
    if (data.motion.channels[i] == "Xrotation")
        return AngleAxisd(n, Vector3d::UnitX());
    if (data.motion.channels[i] == "Yrotation")
        return AngleAxisd(n, Vector3d::UnitY());
    if (data.motion.channels[i] == "Zrotation")
        return AngleAxisd(n, Vector3d::UnitZ());

    cerr << "Unrecognized rotation\n";
    exit(EXIT_FAILURE);
}

void Motion::interpolate(int fps) {
    this->fps = fps;
    int originalFps = (int) round(1 / frameTime);
    cout << "originalFps " << originalFps << endl;
    int framesPerOrig = originalFps / fps;
    cout << "framesPerOrig " << framesPerOrig << endl;
    // interpolate
    for (int i = 1; i < frames.size() - 1; ++i) {
        interpolatedFrames.push_back(frames[i]);
        for (int j = 1; j < framesPerOrig; ++j) {
            interpolatedFrames.push_back(
                    interpolate(
                            frames[i],
                            frames[(i + 1) % frames.size()],
                            (double) j / framesPerOrig
                    ));
        }
    }
    cout << "oFrames.size() " << frames.size() << endl;
    cout << "iFrames.size() " << interpolatedFrames.size() << endl;
}

Frame Motion::interpolate(Frame const &frame1, Frame const &frame2, double lambda) {
    Frame iframe;
    Vector3d oneMinusLambdaT1 = Scaling(1 - lambda) * frame1.translation;
    Vector3d lambdaT2 = Scaling(lambda) * frame2.translation;
    iframe.translation = oneMinusLambdaT1 + lambdaT2;
    for (int i = 0; i < frame1.rotations.size(); ++i) {
        iframe.rotations.push_back(frame1.rotations[i].slerp(lambda, frame2.rotations[i]));
    }
    return iframe;
}
