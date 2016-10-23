#pragma once

#include <string>
#include <Sakura/Bounds.h>
#include <Sakura/SpriteBatch.h>
#include <Sakura/ResourceManager.h>
#include <Sakura/DebugRenderer.h>

enum DamageEffect{
	NORMAL = 0x1, // Normal damage
	FIRE = 0x2, // Periodically damage hull
	EMP = 0x4, // Disable all weapons and shields for the duration
	POWERSHORTAGE = 0x10, // Disable shield and energy weapon recharge
	SUPERCHARGE = 0x20, // FRIENDLY: Boosts shields for the duration
	DAMAGEBOOST = 0x40, // FRIENDLY: Boosts all weapon damage for the duration
	REPAIR = 0x80, // FRIENDLY: Periodically repair the hull
};

enum class ShipType{
	NOSHIP = -1,
	ASSAULT_CARRIER = 0,
	INTERCEPTOR = 1,
	FIGHTER = 2,
	DESTROYER = 3,
	CUTTER = 4,
	CRUISER = 5,
	CORVETTE = 6,
	CARRIER = 7,
	BATTLESHIP = 8,
	CARGO_SHIP = 9
};

class Fleet;
struct Grid;

class Ship
{
public:
	Ship(Grid* grid, Fleet* fleet, Sakura::ResourceManager &resourceManager, std::string team, ShipType shipType, glm::ivec2 position /* Position on GRID */, bool enemy, float speed, int shield, int hull, int shieldDamage, int hullDamage, int damageEffectStrength, DamageEffect damageEffect = NORMAL);
	Ship() { /* Empty */ }

	virtual ~Ship();

	virtual void init(Grid* grid, Fleet* fleet, Sakura::ResourceManager &resourceManager, std::string team, ShipType shipType, glm::ivec2 position /* Position on GRID */, bool enemy, float speed, int shield, int hull, int shieldDamage, int hullDamage, int damageEffectStrength, DamageEffect damageEffect = NORMAL);
	
	virtual void draw(Sakura::SpriteBatch& spriteBatch, bool hover);
	virtual void drawDebug(Sakura::DebugRenderer& debugRenderer);

	void Damage(int hullDamage, int shieldDamage, int effectStrength, DamageEffect statusEffect = NORMAL);

	void ApplyEffect(DamageEffect statusEffect, int effectStrength);

	void damageOther(glm::ivec2 damagePosition);

	/* Return true when finished with concurrent updates */
	bool update(float deltaTime, bool isTurn, Grid* grid);

	void move(const glm::ivec2& newPosition){ m_newPosition = newPosition; }
	const glm::ivec2& getPosition(){ return m_position; };
	const glm::ivec2& getTileSpan(){ return m_tileSpan; }
	const bool& isEnemy(){ return m_enemy; }

	unsigned int getID() const { return m_id; }
	void setID(unsigned int val) { m_id = val; }

	static const std::string getShipName(ShipType shipType);

	bool collidesPoint(const glm::vec2& pointPos){ return m_bounds.pointIntersection(pointPos.x, pointPos.y); }
	bool collidesRect(Sakura::Rect rect){ return m_bounds.calculateRectangleCollision(rect); }
protected:
	// Damage based in integers (hearts)
	int m_shieldDamage = 0;
	int m_hullDamage = 0;
	DamageEffect m_damageEffect = NORMAL;
	int m_damageEffectStrength = 0;
	int m_range;
	float m_speed;

	bool m_enemy = false;

	int m_shieldMax= 5;
	int m_hullMax= 5;
	int m_shield = 5;
	int m_hull = 5;
	DamageEffect m_appliedEffect = NORMAL;
	int m_appliedEffectStrength = 0;

	std::string m_team = "Nuetral";
	ShipType m_shipType;
	unsigned int m_id;

	Fleet* m_fleet;
	glm::ivec2 m_tileSpan;
	glm::ivec2 m_position;
	glm::ivec2 m_newPosition;
	Sakura::Rect m_bounds;
	Sakura::TileSheet m_texture;
	Sakura::TileSheet m_hearts;
	Sakura::GLTexture m_heartContainer;
};