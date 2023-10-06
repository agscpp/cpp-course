#pragma once

#include <array>
#include <cstdint>
#include <queue>

enum class Winner { kFirst, kSecond, kNone };

struct GameResult {
    Winner winner;
    int turn;

    GameResult() : winner(Winner::kNone), turn(0) {
    }
};

GameResult SimulateWarGame(const std::array<int, 6>& first_deck,
                           const std::array<int, 6>& second_deck) {
    GameResult game_result;
    std::queue<int> first_queue;
    std::queue<int> second_queue;
    int32_t max_number_attempts = 1000000;

    for (auto i : first_deck) {
        first_queue.push(i);
    }
    for (auto i : second_deck) {
        second_queue.push(i);
    }

    while (game_result.turn <= max_number_attempts) {
        if (first_queue.empty() || second_queue.empty()) {
            break;
        }

        auto first_card = first_queue.front();
        first_queue.pop();

        auto second_card = second_queue.front();
        second_queue.pop();

        if (first_card == 0 && second_card == 11) {
            first_queue.push(first_card);
            first_queue.push(second_card);
            game_result.winner = Winner::kFirst;
        } else if (first_card == 11 && second_card == 0) {
            second_queue.push(first_card);
            second_queue.push(second_card);
            game_result.winner = Winner::kSecond;
        } else if (first_card > second_card) {
            first_queue.push(first_card);
            first_queue.push(second_card);
            game_result.winner = Winner::kFirst;
        } else if (first_card < second_card) {
            second_queue.push(first_card);
            second_queue.push(second_card);
            game_result.winner = Winner::kSecond;
        }

        ++game_result.turn;
    }

    if (game_result.turn > max_number_attempts) {
        game_result.winner = Winner::kNone;
    }

    return game_result;
}
