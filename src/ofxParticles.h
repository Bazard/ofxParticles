//
//  ofxParticles.h
//  assimpExample
//
//  Created by Timothy Scaffidi on 6/14/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#pragma once
#include <list>

#include "ofMain.h"


inline ofVec3f ofRandVec3f(){
    return ofVec3f(ofRandomf(),ofRandomf(),ofRandomf()).normalize().scale(ofRandomf());
}

class ofxParticle {
private:
	float randomSeed;

public:
    ofVec3f position;
    ofVec3f velocity; // pixels/sec
    ofVec3f acceleration; // pixels/sec/sec
    ofColor color;
    
    ofVec3f rotation;
    ofVec3f rotationalVelocity;
    
    float mass;
    float size;
    float life;
    float lifeStart;
    float dt;
    int particleID;

	bool growIn;
	float growDuration; // percentage of life (0-1)
	bool shrinkOut;
	float shrinkDuration; // percentage of life (0-1) 
	bool fadeWithLife;
    
	bool randomMove;
	ofPoint amplitude;
	ofPoint frequency;

	int textureIndex;

    bool operator < (const ofxParticle &b){
        return position.z < b.position.z;
    }
        
    ofxParticle(){
        position = velocity = acceleration = ofVec3f(0,0,0);
        color = ofColor(255,255,255,255);
        mass = 1.0;
        size = 1.0;
        lifeStart = life = 1.0;
        particleID = 0;
        dt = 1.0/60;

		growIn = false;
		growDuration = 0.5;
		shrinkOut = false;
		shrinkDuration = 0.5;
		fadeWithLife = false;

		randomMove = false;
		amplitude = ofPoint(10,10);
		frequency = ofPoint(5,5);

		randomSeed = ofRandom(ofGetElapsedTimeMillis());

		textureIndex = 0;
    }
        
    ofxParticle(ofVec3f pos, ofVec3f vel, float size_, float life_){
        position = pos;
        velocity = vel;
        acceleration = ofVec3f(0,0,0);
        color = ofColor(255,255,255,255);
        mass = 1.0;
        size = size_;
        lifeStart = life = life_;
        particleID = 0;
        dt = 1.0/60;

		growIn = false;
		growDuration = 0.5;
		shrinkOut = false;
		shrinkDuration = 0.5;
		fadeWithLife = false;

		randomMove = false;
		amplitude = ofPoint(10, 10);
		frequency = ofPoint(5, 5);

		randomSeed = ofRandom(ofGetElapsedTimeMillis());

		textureIndex = 0;
    }
        
    ofxParticle(const ofxParticle &mom){
        position = mom.position;
        velocity = mom.velocity;
        acceleration = mom.acceleration;
        rotation = mom.rotation;
        rotationalVelocity = mom.rotationalVelocity;
        color = mom.color;
        mass = mom.mass;
        size = mom.size;
        life = mom.life;
        lifeStart = mom.lifeStart;
        particleID = mom.particleID;
        dt = 1.0/60;

		growIn = mom.growIn;
		growDuration = mom.growDuration;
		shrinkOut = mom.shrinkOut;
		shrinkDuration = mom.shrinkDuration;
		fadeWithLife = mom.fadeWithLife;

		randomMove = mom.randomMove;
		amplitude = mom.amplitude;
		frequency = mom.frequency;

		randomSeed = ofRandom(ofGetElapsedTimeMillis());

		textureIndex = 0;
    }
        
    ~ofxParticle(){}
        
    ofxParticle & operator=(const ofxParticle &mom){
        if(&mom==this) return *this;
        position = mom.position;
        velocity = mom.velocity;
        acceleration = mom.acceleration;
        rotation = mom.rotation;
        rotationalVelocity = mom.rotationalVelocity;
        color = mom.color;
        mass = mom.mass;
        size = mom.size;
        life = mom.life;
        lifeStart = mom.lifeStart;
        particleID = mom.particleID;
        dt = 1.0/60;

		growIn = mom.growIn;
		growDuration = mom.growDuration;
		shrinkOut = mom.shrinkOut;
		shrinkDuration = mom.shrinkDuration;
		fadeWithLife = mom.fadeWithLife;

		randomMove = mom.randomMove;
		amplitude = mom.amplitude;
		frequency = mom.frequency;

		randomSeed = ofRandom(ofGetElapsedTimeMillis());

		textureIndex = mom.textureIndex;

		return *this;
    }

    void constrainToRect(ofRectangle bounds, const float k, const float dist, const float drag)
    {
        ofPoint minPoint = bounds.getTopLeft();
        ofPoint maxPoint = bounds.getBottomRight();
        float inverse_drag = 1.0f / drag;
        float inverse_mass = 1.0f / mass;
        float spring_constant = inverse_mass * inverse_drag;
        float force;
        ofVec3f dir;
        float dis;

        // left side
        if (position.x < minPoint.x) {
            velocity.x = minPoint.x - position.x;
            position.x = minPoint.x+1;
        }
        if (position.x < minPoint.x + dist) {
            dir = ofVec3f(1,0,0);
            dis = position.x - minPoint.x;
            dis = dist - dis;
            force = -k * dis * spring_constant;
            acceleration += dir*(-force);
        } else {
            // right side
            if (position.x > maxPoint.x) {
                velocity.x = maxPoint.x - position.x;
                position.x = maxPoint.x-1;
            }
            if (position.x > maxPoint.x - dist) {
                dir = ofVec3f(-1,0,0);
                dis = maxPoint.x - position.x;
                dis = dist - dis;
                force =  -k * dis * spring_constant;
                acceleration += dir*(-force);
            }
        }

        // top side
        if (position.y < minPoint.y) {
            velocity.y = minPoint.y - position.y;
            position.y = minPoint.y+1;
        }
        if (position.y < minPoint.y + dist) {
            dir = ofVec3f(0,1,0);
            dis = position.y - minPoint.y;
            dis = dist - dis;
            force = -k * dis * spring_constant;
            acceleration += dir*(-force);
        } else {
            // bottom side
            if (position.y > maxPoint.y) {
                velocity.y = maxPoint.y - position.y;
                position.y = maxPoint.y-1;
            }
            if (position.y > maxPoint.y - dist) {
                dir = ofVec3f(0,-1,0);
                dis = maxPoint.y - position.y;
                dis = dist - dis;
                force =  -k * dis * spring_constant;
                acceleration += dir*(-force);
            }
        }
    }

    void applyForce(ofVec3f force){
        acceleration += force * dt;
    }

    // void spring(ofPoint p, const float k, const float springDist, const float drag){ spring(p, k, springDist, drag); }
    float springBoth(ofxParticle * p, const float k, const float springDist, const float drag = 1.0f, const float springSnap = 1.0f){
        float dist = position.distance(p->position);
        if(dist > springDist * springSnap) return 0.0;

        dist = springDist - dist;
        float force = (-k / (mass * p->mass)) * (dist / drag);

        ofVec3f dir = ((ofVec3f)(p->position - position)).getNormalized();

        acceleration += dir * (force / mass);
        p->acceleration -= dir * (force / p->mass);
        return -force;
    }
        
    void attractTo(ofxParticle p, const float accel, const float minDist, const bool consumeParticle){ attractTo(p.position, accel, minDist, consumeParticle); }
    void attractTo(ofPoint p, const float accel, const float minDist, const bool consumeParticle){
        ofVec3f dir = p-position;
        float dist = dir.length();
        if(dist < minDist){
            dist = minDist;
            if(consumeParticle)
                life = 0;
        }

        dir.normalize().scale(accel*dt);
        acceleration += dir;
    }
        
    void gravitateTo(ofxParticle p, const float gravity, const float minDist, const bool consumeParticle){ gravitateTo(p.position, gravity, p.mass, minDist, consumeParticle); }
    void gravitateTo(ofPoint p, const float gravity, const float mass2, const float minDist, const bool consumeParticle){
        if(p==position) return;
        ofVec3f dir = p-position;
        float dist = dir.length();
        if(dist < minDist){
            dist = minDist;
            if(consumeParticle)
                life = 0;
        }
        dir.normalize().scale( gravity * mass * mass2 * dt * (1.0/(dist)));
        acceleration += dir;
    }

    void gravitateBoth(ofxParticle * p, const float gravity, const float minDist, bool consumeParticle = false) {
        if(p==this) return;
        ofVec3f dir = p->position - position;
        float dist = dir.length();
        if(dist < minDist){
            dist = minDist;
            if(consumeParticle)
                life = 0;
        }
        dir.normalize().scale( gravity * mass * p->mass * dt * (1.0/(dist)));
        acceleration += dir; 
        p->acceleration -= dir;
    }
        
    void rotateAround(ofxParticle p, const float accel, const float minDist, const bool consumeParticle) { rotateAround(p.position, accel, minDist, consumeParticle); }
    void rotateAround(ofPoint p, const float accel, const float minDist, const bool consumeParticle){
        ofVec3f toPoint = position - p;
        ofVec3f dir = ofVec3f(-toPoint.y, toPoint.x, toPoint.z);
        float dist = toPoint.length();
        if(dist < minDist){
            dist = minDist;
            if(consumeParticle)
                life = 0;
        }
        dir.normalize().scale((accel/dist) *dt);
            
        acceleration += dir;
    }
        
    void update(const float timeStep, const float drag){
        dt = timeStep;
        velocity += acceleration * dt;
        velocity -= (velocity * dt * (1.0-drag));
        position += velocity * dt;
		if (randomMove) {
			ofSeedRandom(randomSeed);
			float phase = ofRandom(0, 2*PI);
			ofPoint prevSinus = ofPoint(amplitude.x * sin((life + dt) / lifeStart * frequency.x + phase), amplitude.y * sin((life + dt) / lifeStart * frequency.y + phase));
			ofPoint sinus = ofPoint(amplitude.x * sin(life / lifeStart * frequency.x + phase), amplitude.y * sin(life / lifeStart * frequency.y + phase));
			position = position - prevSinus + sinus;
		}
        acceleration -= acceleration * dt;
        rotation += rotationalVelocity * dt;
            
        life -= dt;
    }
        
    void draw(){
        ofColor c = color;
        c.a = life/lifeStart*color.a;
        ofSetColor(c);
        ofDrawLine(position, position-velocity*dt);
    }
        
    void draw(ofTexture &tex){
        float w = tex.getWidth();
        float h = tex.getHeight();
        if(w > h){
            h = h/w*size;
            w = size;
        }
        else{
            w = w/h*size;
            h = size;
        }

		float evolution = (lifeStart - life) / lifeStart;
		if (growIn && evolution < growDuration) {
			w *= sin(evolution / growDuration * PI/2.0);
			h *= sin(evolution / growDuration * PI/2.0);
		}
		if (shrinkOut && 1 - evolution < shrinkDuration) {
			w *= sin((1-evolution) / shrinkDuration * PI/2.0);
			h *= sin((1-evolution) / shrinkDuration * PI/2.0);
		}

		if (fadeWithLife) {
			ofSetColor(color, sin(evolution * PI) * 255);
		}
		else {
			ofSetColor(color);
		}
		ofPushMatrix();
        ofTranslate(position);
        ofRotateX(rotation.x);
        ofRotateY(rotation.y);
        ofRotateZ(rotation.z);
        tex.draw(w*-0.5,h*-0.5,w,h);
        ofPopMatrix();
    }
        
    bool isAlive() {return life > 0.0;}
        
};
        
class ofxParticleEmitter {
public:
    
	// Unique ID for each emitter (used to create unique seeds for each emitter)
	int emitterId;
	// Total number of emitters created
	static int emittersNum;

	ofxParticleEmitter() : positionStart(), positionEnd(),posSpread(),velocityStart(),velocityEnd(),velSpread(),
    rotation(),rotSpread(),rotVel(),rotVelSpread(),size(1.0),sizeSpread(0.0),
    life(1.0),lifeSpread(0.0),emissionRate(1),color(255,255,255,255),colorSpread(0,0,0,0), lastTimeParticleWasEmitted(ofGetElapsedTimeMillis()),
	growIn(false), growDuration(0.5), shrinkOut(false), shrinkDuration(0.5), fadeWithLife(false),
	randomMove(false), amplitude(ofPoint(10,10)), frequency(ofPoint(5,5))
    {
		emitterId = emittersNum;
		emittersNum++;
	}
    ~ofxParticleEmitter(){}
    ofVec3f positionStart;
    ofVec3f positionEnd;
    ofVec3f posSpread;
    ofVec3f velocityStart;
    ofVec3f velocityEnd;
    ofVec3f velSpread;
    ofVec3f rotation;
    ofVec3f rotSpread;
    ofVec3f rotVel;
    ofVec3f rotVelSpread;
    float size;
    float sizeSpread;
    float life;
    float lifeSpread;
	float emissionRate; // particles emitted / second
	uint64_t lastTimeParticleWasEmitted;
	ofColor color;
    ofColor colorSpread;
			
	bool growIn;
	float growDuration; // percentage of life (0-1)
	bool shrinkOut;
	float shrinkDuration; // percentage of life (0-1) 
	bool fadeWithLife;

	bool randomMove;
	ofPoint amplitude;
	ofPoint frequency;

    ofxParticleEmitter & setPosition(ofVec3f pos){
        positionStart = positionEnd = pos;
        return *this;
    }
    ofxParticleEmitter & setPosition(ofVec3f posStart, ofVec3f posEnd){
        positionStart = posStart;
        positionEnd = posEnd;
        return *this;
    }
    ofxParticleEmitter & setVelocity(ofVec3f vel){
        velocityStart = velocityEnd = vel;
        return *this;
    }
    ofxParticleEmitter & setVelocity(ofVec3f velStart, ofVec3f velEnd){
        velocityStart = velStart;
        velocityEnd = velEnd;
        return *this;
    }
	uint64_t getEmissionIntervalMillis() {
		return (uint64_t)(1.0 / emissionRate * 1000);
	}

	// Reset last time a particle was emitted, to prevent spawning a lot of particles at once when emission was paused for example
	void resetEmissionTimer() {
		lastTimeParticleWasEmitted = ofGetElapsedTimeMillis();
	}

private:
	float emissionInterval;

};

class ofxParticleSystem {
public:
    ofxParticleSystem(){
        numParticles = 0;
        totalParticlesEmitted = 0;
    }
    ~ofxParticleSystem(){}
            
    void addParticles(ofxParticleEmitter & src){
		addParticles(src, 0);
    }

	void addParticles(ofxParticleEmitter & src, vector<ofTexture>& texVec) {
		addParticles(src, texVec.size());
	}
            
    void attractTo(ofPoint p, const float accel, const float minDist, const bool consumeParticle){
        for(list<ofxParticle*>::iterator it = particles.begin(); it != particles.end(); it++){
            (**it).attractTo(p, accel, minDist, consumeParticle);
        }
    }
            
    void gravitateTo(ofPoint p, const float gravity, const float mass, const float minDist, const float consumeParticle){
        for(list<ofxParticle*>::iterator it = particles.begin(); it != particles.end(); it++){
            (**it).gravitateTo(p, gravity, mass, minDist, consumeParticle);
        }
    }
            
    void rotateAround(ofPoint p, const float accel, const float minDist, const float consumeParticle) {
        for(list<ofxParticle*>::iterator it = particles.begin(); it != particles.end(); it++){
            (**it).rotateAround(p, accel, minDist, consumeParticle);
        }
    }
            
    void applyVectorField(float * field, int fieldWidth, int fieldHeight, int numComponents, ofRectangle areaOfInfluence, float force) {
        for(list<ofxParticle*>::iterator it = particles.begin(); it != particles.end(); it++){
            ofxParticle & p = (**it);
            ofVec2f pos(p.position.x,p.position.y);
            if(areaOfInfluence.inside(pos)){
                int x = (int)ofMap(pos.x, areaOfInfluence.getMinX(), areaOfInfluence.getMaxX(), 0, fieldWidth-1);
                int y = (int)ofMap(pos.y, areaOfInfluence.getMinY(), areaOfInfluence.getMaxY(), 0, fieldHeight-1);
                int index = (x+y*fieldWidth)*numComponents;
                ofVec2f dir(field[index],field[index+1]);
                dir.scale(force);
//                                    cout << "(" << dir.x << "," << dir.y << ")\n";
                p.applyForce(dir);
            }
        }
    }
            
    int update(float timeStep, float drag){
        int particlesRemoved = 0;
        for(list<ofxParticle*>::iterator it = particles.begin(); it != particles.end(); ++it){
            if((**it).isAlive()){
                (**it).update(timeStep, drag);
            }
            else{
                ofxParticle * p = (*it);
                it = particles.erase(it);
                delete p;
                particlesRemoved++;

				if (it == particles.end()) break;
            }
        }
        numParticles-=particlesRemoved;
        return particlesRemoved;
    }
            
    void draw(){
        for(list<ofxParticle*>::iterator it = particles.begin(); it != particles.end(); it++){
            (**it).draw();
        }
    }
    void draw(ofTexture &tex){
        //                particles.sort();
        for(list<ofxParticle*>::iterator it = particles.begin(); it != particles.end(); it++){
            (**it).draw(tex);
        }
    }
            
    void draw(ofTexture &tex, ofTexture &tex2){
        //                particles.sort();
        for(list<ofxParticle*>::iterator it = particles.begin(); it != particles.end(); it++){
            int index = (**it).particleID;
            if(index%2==0)
                (**it).draw(tex);
            else
                (**it).draw(tex2);
        }
    }

	void draw(vector<ofTexture>& texVec) {
		//                particles.sort();
		for (list<ofxParticle*>::iterator it = particles.begin(); it != particles.end(); it++) {
			int index = (**it).particleID;
			if ((**it).textureIndex < 0 || (**it).textureIndex >= texVec.size()) {
				(**it).draw(texVec[index%texVec.size()]);
			}
			else {
				(**it).draw(texVec[(**it).textureIndex]);
			}
		}
	}
            
    int getNumParticles(){return numParticles;}
            
private:
    list<ofxParticle*> particles;
    int numParticles;
    int totalParticlesEmitted;

	void addParticles(ofxParticleEmitter & src, int texNum) {
		ofSeedRandom();
		int particlesToEmit = 0;
		if (ofGetElapsedTimeMillis() - src.lastTimeParticleWasEmitted > src.getEmissionIntervalMillis()) {
			particlesToEmit = (ofGetElapsedTimeMillis() - src.lastTimeParticleWasEmitted) / src.getEmissionIntervalMillis();
			src.lastTimeParticleWasEmitted = ofGetElapsedTimeMillis();
		}

		for (int i = 0; i<particlesToEmit; i++) {
			// Seed random for each particle
			srand(GetTickCount() * (1 + src.emitterId + i));

			ofVec3f pos = src.positionStart;
			ofVec3f vel = src.velocityStart;
			if (src.positionEnd != src.positionStart || src.velocityStart != src.velocityEnd) {
				float rf = ofRandomuf();
				pos = src.positionStart.getInterpolated(src.positionEnd, rf);
				vel = src.velocityStart.getInterpolated(src.velocityEnd, rf);
			}
			ofVec3f p = pos + ofRandVec3f()*src.posSpread;
			ofVec3f v = vel + ofRandVec3f()*src.velSpread;
			float s = src.size + ofRandomf()*src.sizeSpread;
			float l = src.life + ofRandomf()*src.lifeSpread;
			ofxParticle * par = new ofxParticle(p, v, s, l);
			par->rotation = src.rotation + ofRandVec3f()*src.rotSpread;
			par->rotationalVelocity = src.rotVel + ofRandVec3f()*src.rotVelSpread;
			par->particleID = totalParticlesEmitted + i;
			par->growIn = src.growIn;
			par->growDuration = src.growDuration;
			par->shrinkOut = src.shrinkOut;
			par->shrinkDuration = src.shrinkDuration;
			par->fadeWithLife = src.fadeWithLife;
			par->randomMove = src.randomMove;
			par->frequency = src.frequency;
			par->amplitude = src.amplitude;
			ofColor pColor = src.color;
			if (src.colorSpread != ofColor(0, 0, 0, 0)) {
				pColor.r = ofClamp(pColor.r + ofRandomf()*src.colorSpread.r, 0, 255);
				pColor.g = ofClamp(pColor.g + ofRandomf()*src.colorSpread.g, 0, 255);
				pColor.b = ofClamp(pColor.b + ofRandomf()*src.colorSpread.b, 0, 255);
				pColor.a = ofClamp(pColor.a + ofRandomf()*src.colorSpread.a, 0, 255);
			}
			par->color = pColor;
			if (texNum <= 0) {
				par->textureIndex = 0;
			}
			else {
				par->textureIndex = rand() % (int)(texNum);
			}
			particles.push_back(par);
		}
		numParticles += particlesToEmit;
		totalParticlesEmitted += particlesToEmit;
	}
};
