#include <criterion/criterion.h>
#include <chrono>
#include <cstdlib>
#include <memory>
#include <string>
#include <vector>

#include "commands/CommandFactory.hpp"
#include "commands/HelloCommand.hpp"
#include "game/Game.hpp"
#include "game/Inventory.hpp"
#include "game/Player.hpp"
#include "game/Timeline.hpp"
#include "utils/BroadcastDirection.hpp"
#include "utils/ServerArgs.hpp"

Test(network_inventory, stores_and_formats_resources)
{
    Inventory inventory;

    cr_assert_eq(inventory.getRessourcesQuantity(Food), 0);
    inventory.addRessources(Food, 3);
    cr_assert_eq(inventory.getRessourcesQuantity(Food), 3);
    cr_assert(inventory.removeRessources(Food, 2));
    cr_assert_eq(inventory.getRessourcesQuantity(Food), 1);
    cr_assert_not(inventory.removeRessources(Food, 2));
    cr_assert_eq(inventory.toString(),
        "[food 1, linemate 0, deraumere 0, sibur 0, mendiane 0, phiras 0, thystame 0]");
}

Test(network_player, moves_on_toroidal_map)
{
    Player player;

    player.goForward(4, 4);
    cr_assert_eq(player.getPosition(),
        (std::make_pair<std::size_t, std::size_t>(0, 3)));
    player.turnRight();
    player.goForward(4, 4);
    cr_assert_eq(player.getPosition(),
        (std::make_pair<std::size_t, std::size_t>(1, 3)));
    player.turnRight();
    player.goForward(4, 4);
    cr_assert_eq(player.getPosition(),
        (std::make_pair<std::size_t, std::size_t>(1, 0)));
    player.turnLeft();
    cr_assert_eq(player.getDirection(), East);
}

Test(network_game, manages_eggs)
{
    Game game(4, 3);
    int first = game.addEgg(7, 1, 2, "red");
    int second = game.addEgg(8, 1, 2, "blue");

    cr_assert_eq(game.getWidth(), 4);
    cr_assert_eq(game.getHeight(), 3);
    cr_assert_neq(first, second);
    cr_assert_not_null(game.findEggForTeam("red"));
    cr_assert_null(game.findEggForTeam("missing"));

    std::vector<GameEgg> removed = game.removeEggsAt(1, 2);

    cr_assert_eq(removed.size(), 2);
    cr_assert(game.getEggs().empty());
}

Test(network_broadcast, computes_relative_directions)
{
    cr_assert_eq(BroadcastDirection::compute(2, 2, 2, 2, North, 5, 5), 0);
    cr_assert_eq(BroadcastDirection::compute(2, 1, 2, 2, North, 5, 5), 1);
    cr_assert_eq(BroadcastDirection::compute(2, 1, 2, 2, East, 5, 5), 3);
    cr_assert_eq(BroadcastDirection::compute(4, 2, 0, 2, North, 5, 5), 3);
}

Test(network_timeline, pops_actions_in_order)
{
    Timeline timeline;
    auto now = std::chrono::system_clock::now();
    std::unique_ptr<ICommand> late = std::make_unique<HelloCommand>();
    std::unique_ptr<ICommand> early = std::make_unique<HelloCommand>();

    timeline.addAction(now + std::chrono::milliseconds(50), late, "late", 4);
    timeline.addAction(now + std::chrono::milliseconds(10), early, "early", 2);

    cr_assert_eq(timeline.size(), 2);
    cr_assert_eq(timeline.pop()->args, "early");
    timeline.removePollIndex(4);

    std::shared_ptr<Action> action = timeline.pop();

    cr_assert_eq(action->poll_index, -2);
    cr_assert_eq(timeline.size(), 0);
}

Test(network_args, parses_server_arguments)
{
    const char *raw[] = {
        "zappy_server", "-p", "4242", "-x", "10", "-y", "11",
        "-c", "3", "-f", "100", "-n", "red", "blue"
    };

    optind = 1;
    ServerArgs args(14, const_cast<char **>(raw));

    cr_assert_eq(args.getPort(), 4242);
    cr_assert_eq(args.getWidth(), 10);
    cr_assert_eq(args.getHeight(), 11);
    cr_assert_eq(args.getClientNumber(), 3);
    cr_assert_eq(args.getFrequency(), 100);
    cr_assert_eq(args.getTeamsName().size(), 2);
}

Test(network_args, rejects_invalid_map_size)
{
    const char *tooSmall[] = {
        "zappy_server", "-p", "4242", "-x", "9", "-y", "10",
        "-c", "3", "-f", "100", "-n", "red"
    };
    const char *tooLarge[] = {
        "zappy_server", "-p", "4242", "-x", "42", "-y", "50",
        "-c", "3", "-f", "100", "-n", "red"
    };

    cr_assert_throw(ServerArgs(12, const_cast<char **>(tooSmall)),
        ServerArgs::ServerArgsException);
    cr_assert_throw(ServerArgs(12, const_cast<char **>(tooLarge)),
        ServerArgs::ServerArgsException);
}

Test(network_args, rejects_non_numeric_values)
{
    const char *raw[] = {
        "zappy_server", "-p", "4242", "-x", "abc", "-y", "10",
        "-c", "3", "-f", "100", "-n", "red"
    };

    cr_assert_throw(ServerArgs(12, const_cast<char **>(raw)),
        ServerArgs::ServerArgsException);
}

Test(network_args, rejects_invalid_port_and_frequency)
{
    const char *badPort[] = {
        "zappy_server", "-p", "0", "-x", "10", "-y", "10",
        "-c", "3", "-f", "100", "-n", "red"
    };
    const char *badFrequency[] = {
        "zappy_server", "-p", "4242", "-x", "10", "-y", "10",
        "-c", "3", "-f", "1001", "-n", "red"
    };

    cr_assert_throw(ServerArgs(12, const_cast<char **>(badPort)),
        ServerArgs::ServerArgsException);
    cr_assert_throw(ServerArgs(12, const_cast<char **>(badFrequency)),
        ServerArgs::ServerArgsException);
}

Test(network_args, rejects_missing_or_unknown_options)
{
    const char *missingValue[] = {
        "zappy_server", "-p", "4242", "-x", "10", "-y", "10",
        "-c", "3", "-f", "100", "-n"
    };
    const char *unknownOption[] = {
        "zappy_server", "-p", "4242", "-x", "10", "-y", "10",
        "-c", "3", "-f", "100", "-z", "red"
    };

    cr_assert_throw(ServerArgs(11, const_cast<char **>(missingValue)),
        ServerArgs::ServerArgsException);
    cr_assert_throw(ServerArgs(12, const_cast<char **>(unknownOption)),
        ServerArgs::ServerArgsException);
}

Test(network_args, rejects_reserved_or_duplicate_team_names)
{
    const char *reserved[] = {
        "zappy_server", "-p", "4242", "-x", "10", "-y", "10",
        "-c", "3", "-f", "100", "-n", "GRAPHIC"
    };
    const char *duplicate[] = {
        "zappy_server", "-p", "4242", "-x", "10", "-y", "10",
        "-c", "3", "-f", "100", "-n", "red", "red"
    };

    cr_assert_throw(ServerArgs(12, const_cast<char **>(reserved)),
        ServerArgs::ServerArgsException);
    cr_assert_throw(ServerArgs(14, const_cast<char **>(duplicate)),
        ServerArgs::ServerArgsException);
}

Test(network_command_factory, creates_known_commands)
{
    cr_assert_not_null(CommandFactory::createCommand("Forward"));
    cr_assert_not_null(CommandFactory::createCommand("msz"));
    cr_assert_null(CommandFactory::createCommand("unknown"));
}
