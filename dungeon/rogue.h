#pragma once

#include "dungeon.h"

#include <cstddef>
#include <list>
#include <map>
#include <queue>
#include <unordered_set>

namespace {
const Room* CheckKey(std::list<Door*>* doors, const std::string& key) {
    for (auto it = doors->begin(); it != doors->end(); ++it) {
        auto* door = *it;
        if (door->TryOpen(key)) {
            doors->erase(it);
            return door->GoThrough();
        }
    }
    return nullptr;
}

const Room* TryOpenDoor(Door* door, std::unordered_set<std::string>* keys) {
    if (door->IsOpen()) {
        return door->GoThrough();
    }

    for (auto key : *keys) {
        if (door->TryOpen(key)) {
            keys->erase(key);
            return door->GoThrough();
        }
    }

    return nullptr;
}
}  // namespace

const Room* FindFinalRoom(const Room* starting_room) {
    std::list<Door*> doors;
    std::queue<const Room*> rooms;
    std::unordered_set<const Room*> visited;
    std::unordered_set<std::string> keys;

    rooms.push(starting_room);
    while (!rooms.empty()) {
        const auto* curr_room = rooms.back();

        visited.insert(curr_room);
        rooms.pop();

        if (curr_room->IsFinal()) {
            return curr_room;
        }

        for (size_t i = 0; i < curr_room->NumKeys(); ++i) {
            const auto& key = curr_room->GetKey(i);
            const auto* room = CheckKey(&doors, key);
            if (room) {
                if (room->IsFinal()) {
                    return room;
                }
                if (!visited.contains(room)) {
                    rooms.push(room);
                }
            } else {
                keys.insert(std::move(key));
            }
        }

        for (size_t i = 0; i < curr_room->NumDoors(); ++i) {
            auto* door = curr_room->GetDoor(i);
            const auto* room = TryOpenDoor(door, &keys);
            if (room) {
                if (room->IsFinal()) {
                    return room;
                }
                if (!visited.contains(room)) {
                    rooms.push(room);
                }
            } else {
                doors.push_back(door);
            }
        }
    }

    return nullptr;
}