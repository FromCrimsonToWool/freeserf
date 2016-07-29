/*
 * map-generator.h - Map generator header
 *
 * Copyright (C) 2013-2016  Jon Lund Steffensen <jonlst@gmail.com>
 *
 * This file is part of freeserf.
 *
 * freeserf is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * freeserf is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with freeserf.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef SRC_MAP_GENERATOR_H_
#define SRC_MAP_GENERATOR_H_

#include "src/map.h"
#include "src/random.h"

/* Interface for map generators. */
class MapGenerator {
 public:
  virtual ~MapGenerator() {}
  virtual void generate() = 0;

  virtual int get_height(map_pos_t pos) const = 0;
  virtual map_terrain_t get_type_up(map_pos_t pos) const = 0;
  virtual map_terrain_t get_type_down(map_pos_t pos) const = 0;
  virtual map_obj_t get_obj(map_pos_t pos) const = 0;
  virtual ground_deposit_t get_resource_type(map_pos_t pos) const = 0;
  virtual int get_resource_amount(map_pos_t pos) const = 0;
};

/* Classic map generator as in original game. */
class ClassicMapGenerator : public MapGenerator {
 public:
  static const int default_max_lake_area;
  static const int default_water_level;
  static const int default_terrain_spikyness;

  ClassicMapGenerator(map_t *map, const random_state_t &random);
  virtual ~ClassicMapGenerator();
  void init(int height_generator, bool preserve_bugs,
            int max_lake_area = default_max_lake_area,
            int water_level = default_water_level,
            int terrain_spikyness = default_terrain_spikyness);
  void generate();

  int get_height(map_pos_t pos) const { return tiles[pos].height; }
  map_terrain_t get_type_up(map_pos_t pos) const {
    return tiles[pos].type_up; }
  map_terrain_t get_type_down(map_pos_t pos) const {
    return tiles[pos].type_down; }
  map_obj_t get_obj(map_pos_t pos) const { return tiles[pos].obj; }
  ground_deposit_t get_resource_type(map_pos_t pos) const {
    return tiles[pos].resource_type; }
  int get_resource_amount(map_pos_t pos) const {
    return tiles[pos].resource_amount; }

 protected:
  struct MapTile {
    int height;
    map_terrain_t type_up;
    map_terrain_t type_down;
    map_obj_t obj;
    ground_deposit_t resource_type;
    int resource_amount;
  };

  map_t *map;
  random_state_t rnd;

  int tile_count;
  MapTile *tiles;
  int height_generator;
  bool preserve_bugs;

  int16_t water_level;
  int16_t max_lake_area;
  int terrain_spikyness;

  uint16_t random_int();

  bool is_water_tile(map_pos_t pos) const;
  bool is_in_water(map_pos_t pos) const;

  void init_heights_squares();
  int calc_height_displacement(int avg, int base, int offset);
  void init_heights_midpoints();
  void init_heights_diamond_square();
  bool adjust_map_height(int h1, int h2, map_pos_t pos);
  void clamp_heights();

  int expand_level_area(map_pos_t pos, int limit, int r);
  void init_level_area(map_pos_t pos);
  void init_sea_level();
  void heights_rebase();
  void init_types();
  void init_types_2_sub();
  void init_types_2();
  void heights_rescale();
  void init_types_shared_sub(map_terrain_t old, map_terrain_t seed,
                             map_terrain_t new_);
  void init_lakes();
  void init_types4();
  map_pos_t lookup_pattern(int col, int row, int index);
  int init_desert_sub1(map_pos_t pos);
  int init_desert_sub2(map_pos_t pos);
  void init_desert();
  void init_desert_2_sub();
  void init_desert_2();
  void init_crosses();
  bool hexagon_types_in_range(map_pos_t pos, map_terrain_t min,
                              map_terrain_t max);
  map_pos_t lookup_rnd_pattern(int col, int row, int mask);
  void init_objects_shared(int num_clusters, int objs_in_cluster, int pos_mask,
                           map_terrain_t type_min, map_terrain_t type_max,
                           int obj_base, int obj_mask);
  void init_trees_1();
  void init_trees_2();
  void init_trees_3();
  void init_trees_4();
  void init_stone_1();
  void init_stone_2();
  void init_dead_trees();
  void init_large_boulders();
  void init_water_trees();
  void init_stubs();
  void init_small_boulders();
  void init_cadavers();
  void init_cacti();
  void init_water_stones();
  void init_palms();
  void init_resources_shared_sub(int iters, int col, int row, int *index,
                                 int amount, ground_deposit_t type);
  void init_resources_shared(int num_clusters, ground_deposit_t type,
                             map_terrain_t min, map_terrain_t max);
  void init_resources();
  void init_clean_up();
  void init_sub();
};

/* Classic map generator that generates identical maps for missions. */
class ClassicMissionMapGenerator : public ClassicMapGenerator {
 public:
  ClassicMissionMapGenerator(map_t *map, const random_state_t &random);
  void init();
};

#endif  // SRC_MAP_GENERATOR_H_
