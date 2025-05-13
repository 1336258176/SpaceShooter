#include "entity.hpp"

void parseConfigFile(std::string_view file_path) {
  auto config = toml::parse_file(file_path);

  PlayerMaxHP = config["Player"]["MaxHP"].value_or(0);
  PlayerBulletDamage = config["Player"]["BulletDamage"].value_or(0);
  PlayerSpeed = config["Player"]["Speed"].value_or(0);
  PlayerBulletSpeed = config["Player"]["BulletSpeed"].value_or(0);
  PlayerShootCooldown = config["Player"]["ShootCooldown"].value_or(0);

  EnemyMaxHP = config["Enemy"]["MaxHP"].value_or(0);
  EnemyBulletDamage = config["Enemy"]["BulletDamage"].value_or(0);
  EnemySpeed = config["Enemy"]["Speed"].value_or(0);
  EnemyBulletSpeed = config["Enemy"]["BulletSpeed"].value_or(0);
  EnemyShootCooldown = config["Enemy"]["ShootCooldown"].value_or(0);

  ItemCollisionCount = config["GameConfig"]["ItemCollisionCount"].value_or(0);
  ItemSpeed = config["GameConfig"]["ItemSpeed"].value_or(0);
  NearBackgroundSpeed = config["GameConfig"]["NearBackgroundSpeed"].value_or(0);
  FarBackgroundSpeed = config["GameConfig"]["FarBackgroundSpeed"].value_or(0);
  GenerateLifeItemProbability = config["GameConfig"]["GenerateLifeItemProbability"].value_or(0.f);
  DefeatEnemyPoints = config["GameConfig"]["DefeatEnemyPoints"].value_or(0);
  CollideEnemyCosts = config["GameConfig"]["CollideEnemyCosts"].value_or(0);

  using namespace std::literals;
  PlayerTexturePath = config["Texture"]["Player"].value_or(""s);
  PlayerBulletTexturePath = config["Texture"]["PlayerBullet"].value_or(""s);
  EnemyTexturePath = config["Texture"]["Enemy"].value_or(""s);
  EnemyBulletTexturePath = config["Texture"]["EnemyBullet"].value_or(""s);
  ExplosionTexturePath = config["Texture"]["Explosion"].value_or(""s);
  LifeItemTexturePath = config["Texture"]["LifeItem"].value_or(""s);
  TimeItemTexturePath = config["Texture"]["TimeItem"].value_or(""s);
  ShieldItemTexturePath = config["Texture"]["ShieldItem"].value_or(""s);
  NearStarBackgroundPath = config["Texture"]["NearStarBackground"].value_or(""s);
  FarStarBackgroundPath = config["Texture"]["FarStarBackground"].value_or(""s);
  HPUITexturePath = config["Texture"]["HPUI"].value_or(""s);

  TextFontPath = config["Font"]["TextFont"].value_or(""s);
  TitleFontPath = config["Font"]["TitleFont"].value_or(""s);
}