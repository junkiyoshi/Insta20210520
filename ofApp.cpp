#include "ofApp.h"	

//--------------------------------------------------------------
void ofApp::setup() {

	ofSetFrameRate(60);
	ofSetWindowTitle("openFrameworks");

	ofBackground(0);
	ofEnableBlendMode(ofBlendMode::OF_BLENDMODE_ADD);
	ofNoFill();

	ofColor color;
	vector<int> hex_list = { 0xf72585, 0xb5179e, 0x7209b7, 0x480ca8, 0x3f37c9, 0x4cc9f0 };
	for (auto hex : hex_list) {

		color.setHex(hex);
		this->base_color_list_1.push_back(color);
	}

	hex_list = { 0x1f005c, 0x5b0060, 0x870160, 0xac255e, 0xca485c, 0xe16b5c, 0xf39060, 0xffb56b };
	for (auto hex : hex_list) {

		color.setHex(hex);
		this->base_color_list_2.push_back(color);
	}
}

//--------------------------------------------------------------
void ofApp::update() {

	this->location_list.clear();

	for (int i = 0; i < 2; i++) {

		auto location = glm::vec3(200 * cos((ofGetFrameNum() * 0.75 + i * 180) * DEG_TO_RAD), 200 * sin((ofGetFrameNum() * 0.75 + i * 180) * DEG_TO_RAD),  0);

		for (int deg = 0; deg < 360; deg++) {

			this->location_list.push_back(location + glm::vec3(80 * cos(deg * DEG_TO_RAD), 80 * sin(deg * DEG_TO_RAD), 0));
		}
	}

	int radius = 7;
	while (this->log_list.size() < 2000) {

		int location_index = ofRandom(this->location_list.size());
		vector<glm::vec2> log;
		log.push_back(this->location_list[location_index]);
		this->log_list.push_back(log);

		if (location_index < 360) {

			this->color_list.push_back(this->base_color_list_1[(int)ofRandom(this->base_color_list_1.size())]);
		}
		else {

			this->color_list.push_back(this->base_color_list_2[(int)ofRandom(this->base_color_list_2.size())]);
		}

		this->life_list.push_back(ofRandom(30, 50));
	}

	for (int i = this->log_list.size() - 1; i >= 0; i--) {

		this->life_list[i] -= 1;
		if (this->life_list[i] < 0) {

			this->log_list.erase(this->log_list.begin() + i);
			this->color_list.erase(this->color_list.begin() + i);
			this->life_list.erase(this->life_list.begin() + i);

			continue;
		}

		auto deg = ofMap(ofNoise(glm::vec3(this->log_list[i].back() * 0.008, ofGetFrameNum() * 0.003)), 0, 1, -360, 360);
		this->log_list[i].push_back(this->log_list[i].back() + glm::vec2(radius * cos(deg * DEG_TO_RAD), radius * sin(deg * DEG_TO_RAD)));
		while (this->log_list[i].size() > 100) {

			this->log_list[i].erase(this->log_list[i].begin());
		}
	}
}

//--------------------------------------------------------------
void ofApp::draw() {

	ofTranslate(ofGetWindowSize() * 0.5);

	for (int i = 0; i < this->log_list.size(); i++) {

		ofSetColor(this->color_list[i]);

		if (this->life_list[i] > 30) {

			ofSetLineWidth(1.2);
		}
		else {

			ofSetLineWidth(ofMap(this->life_list[i], 0, 30, 0, 1.2));
		}

		ofBeginShape();
		ofVertices(this->log_list[i]);
		ofEndShape();
	}

	ofSetLineWidth(2);

	ofSetColor(this->base_color_list_1.back());
	ofDrawCircle(glm::vec2(200 * cos((ofGetFrameNum() * 0.75) * DEG_TO_RAD), 200 * sin((ofGetFrameNum() * 0.75) * DEG_TO_RAD)), 80);

	ofSetColor(this->base_color_list_2.back());
	ofDrawCircle(glm::vec2(200 * cos((ofGetFrameNum() * 0.75 + 180) * DEG_TO_RAD), 200 * sin((ofGetFrameNum() * 0.75 + 180) * DEG_TO_RAD)), 80);

}

//--------------------------------------------------------------
int main() {

	ofSetupOpenGL(720, 720, OF_WINDOW);
	ofRunApp(new ofApp());
}