#pragma once

#include <string>
#include <Sakura/Bounds.h>
#include <Sakura/SpriteBatch.h>
#include <Sakura/ResourceManager.h>
#include <Sakura/DebugRenderer.h>

#include <random>
#include <ctime>

enum Effect{
	NORMAL, // Normal damage
	FIRE, // Periodically damage hull
	EMP, // Disable all weapons and shields for the duration
	POWERSHORTAGE, // Disable shield and energy weapon recharge
	SUPERCHARGE, // FRIENDLY: Boosts shields for the duration
	DAMAGEBOOST, // FRIENDLY: Boosts all weapon damage for the duration
	REPAIR, // FRIENDLY: Periodically repair the hull
};

struct DamageEffect{
	DamageEffect() : effect(NORMAL), strength(0.0f), chance(0.0f), duration(0){ /* Empty */ }
	DamageEffect(Effect newEffect, float newStrength, float newChance, int newDuration) : 
		effect(newEffect), strength(newStrength), chance(newChance), duration(newDuration){ /* Empty */}
	Effect effect;
	float strength;
	float chance;
	int duration;

	bool applyEffect(){
		std::mt19937 gen((unsigned int)time(nullptr));
		std::uniform_real_distribution<float> percent(0.0f, 1.0f);
		if (percent(gen) <= chance){
			return true;
		}
		return false;
	}
};

enum class ShipType{
	NOSHIP = -1,
	CUTTER = 0,
	FIGHTER = 1,
	INTERCEPTOR = 2,
	BOMBER = 3,
	CORVETTE = 4,
	CRUISER = 5,
	CARRIER = 6,
	DESTROYER = 7,
	ASSAULT_CARRIER = 8,
	BATTLESHIP = 9,
	COMMANDSHIP = 10
};

class Fleet;
struct Grid;

typedef int CP;

class Ship
{
public:
	Ship(Grid* grid, Fleet* fleet, Sakura::ResourceManager &resourceManager, std::string team, 
		ShipType shipType, glm::ivec2 position /* Position on GRID */, bool enemy, int speed, 
		int shield, int hull, int shieldDamage, int hullDamage, int range, CP CPcost, DamageEffect damageEffect);
	Ship() { /* Empty */ }

	virtual ~Ship();

	void init(Grid* grid, Fleet* fleet, Sakura::ResourceManager &resourceManager, std::string team, 
		ShipType shipType, glm::ivec2 position /* Position on GRID */, bool enemy, int speed, int shield, 
		int hull, int shieldDamage, int hullDamage, int range, CP CPcost, DamageEffect damageEffect);
	
	virtual int destroy();

	virtual void draw(Sakura::SpriteBatch& spriteBatch, Grid* grid, bool hover);
	void drawTravelTrail(Sakura::SpriteBatch& spriteBatch, Grid* grid);
	void drawAttackTrail(Sakura::SpriteBatch& spriteBatch, Grid* grid);
	void drawDebug(Sakura::DebugRenderer& debugRenderer);

	virtual int Damage(int hullDamage, int shieldDamage, DamageEffect statusEffect);

	void ApplyEffect(DamageEffect statusEffect);

	virtual void damageOther(Ship* otherShip);

	/* returns true if move is finished, false if the move still needs to be updated */
	virtual int update(float deltaTime, Grid* grid);
	virtual bool updateMove(float deltaTime, Grid* grid);
	virtual void updateAttack();
	virtual void endTurn();
	void calculateFriendlyEffects();
	int calculateBadEffects();

	void move(const glm::ivec2& newPosition, Grid* grid, Fleet* enemyFleet);
	const glm::ivec2& getPosition(){ return m_position; };
	const glm::ivec2& getTileSpan(){ return m_tileSpan; }
	const bool& isEnemy(){ return m_enemy; }

	unsigned int getID() const { return m_id; }
	void setID(unsigned int val) { m_id = val; }

	static const std::string getShipName(ShipType shipType);
	static const CP getTypeCost(ShipType shipType);
	const CP& getCost() const{ return m_CPcost; }

	void setSelected(bool selected){ m_isSelected = selected; }
	bool isSelected(){ return m_isSelected; }

	bool isMoveFinished(){ return m_moveFinished; }
	void queueAttack(Ship* ship){ m_queuedAttack = ship;}

	bool collidesPoint(const glm::vec2& pointPos){ return m_bounds.pointIntersection(pointPos.x, pointPos.y); }
	bool collidesRect(Sakura::Rect& rect){ return m_bounds.intersect(rect); }
	ShipType getShipType() const { return m_shipType; }
	void setShipType(ShipType shipType) { m_shipType = shipType; }
	const int& getAttackRange() { return m_range; }
	const int& getSpeed() { return m_speed; }
	const int& getHullDamage(){ return m_hullDamage; }
	const int& getShieldDamage(){ return m_shieldDamage; }

	float getShipThreat(){ return m_threatLevel; }
	int getShipHull(){ return m_hull; }
	int getShipHullMax(){ return m_hullMax; }
	int getShipShield(){ return m_shield; }
	int getShipShieldMax(){ return m_shieldMax; }
	void scaleStrength(float healthScaler, float damageScaler);

	//Use void* to reference anytype of data such as, 
	// to turn on cloaking, (interceptor)
	// the position of the ship to spawn, (Carriers)
	// or how many ships to add to group (Cutters)
	// Effectively a callback function for data modifications in unique ships
	// Returns arbitrary negetive int so no conflicts with the callback return
	// <3 Callbacks
	virtual int callUnique(void* data) { return -32; }

	friend bool operator>(Ship& s1, Ship& s2){
		return s1.getShipThreat() > s2.getShipThreat();
	}
	friend bool operator<(Ship& s1, Ship& s2){
		return s1.getShipThreat() < s2.getShipThreat();
	}
	friend bool operator<=(Ship& s1, Ship& s2){
		return !operator>(s1,s2);
	}

protected:
	// Damage based in integers (hearts)
	int m_shieldDamage = 0; //+
	int m_hullDamage = 0; //+
	DamageEffect m_damageEffect;
	int m_range; //+
	int m_speed; //+
	float m_threatLevel = 0.0f;

	bool m_enemy = false;
	CP m_CPcost = 0;
	bool m_canAttack = true;

	int m_shieldMax= 5;
	int m_hullMax= 5;
	int m_shield = 5; //+
	int m_hull = 5; //+
	std::vector<DamageEffect> m_appliedEffects;

	std::string m_team = "Nuetral";
	ShipType m_shipType;
	unsigned int m_id;
	bool m_hasUpdatedOnce = false;
	float m_moveCounter;
	bool m_moveFinished = false;

	Fleet* m_fleet;
	glm::ivec2 m_tileSpan;
	glm::ivec2 m_position;
	glm::ivec2 m_newPosition;
	glm::ivec2 m_newPositionClamped;
	Ship* m_queuedAttack = nullptr;
	Sakura::Rect m_bounds;
	Sakura::TileSheet m_texture;
	Sakura::TileSheet m_hearts;
	Sakura::TileSheet m_trailMarkers;
	Sakura::TileSheet m_attackMarkers;
	Sakura::GLTexture m_shieldTex;

	float m_selectedSin = 0;
	bool m_isSelected = false;
};