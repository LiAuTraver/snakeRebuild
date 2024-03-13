module;

export module ancillarycat.snake;

import "config.hpp";
import <conio.h>;
import <Windows.h>;
import ancillarycat.console;
import ancillarycat.windows.api;
import ancillarycat.leaderboard;
import std;

export enum class direction;
export class Generator;
export class Entity;
export class Food;
export class Snake;
export class Node;
export template <class _MyBase, class _MyDerived> bool instanceof(_MyBase*);
NO_EXPORT int checkInvalidPosition(Entity&, Entity&);
NO_EXPORT int checkSnakeFood(const Snake&, const Food&);
NO_EXPORT int checkOutofBound(const Entity&);
constinit int time = 0; // for debugging
namespace game
{
export int snakeGame();
}
// definition goes below.

/*
* @brief check whether the base pointer is an instance of the derived class
* @param base the base pointer
* @return true if the base pointer is an instance of the derived class, otherwise false
* @note this can be viewed as the keyword `instanceof` in Java
*/
template <class _MyBase, class _MyDerived>
inline bool instanceof(_MyBase* base) {
	return dynamic_cast<_MyDerived*>(base) != nullptr;
}
enum class direction : int {
	NO_DIRECTION = 0,
	UP,
	DOWN,
	LEFT,
	RIGHT
};

class Generator {
public:
	[[nodiscard]] static direction direct() noexcept
	{
		std::mt19937 gen(device_());
		std::uniform_int_distribution<int> distribution(1, 4);
		return static_cast<direction>(distribution(gen));
	}
	[[nodiscard]] static std::pair<short, short> coordinate(const short& y_lower_bound, const short& y_upper_bound,
		const short& x_lower_bound,
		const short& x_upper_bound) noexcept
	{
		std::mt19937 gen(device_());
		std::uniform_int_distribution<short> y_distribution(y_lower_bound, y_upper_bound);
		std::uniform_int_distribution<short> x_distribution(x_lower_bound, x_upper_bound);
		return std::make_pair(y_distribution(gen), x_distribution(gen));
	}
	template <typename _T>
	[[nodiscard]] static _T random(const _T& lower_bound, const _T& upper_bound) noexcept
	{
		std::mt19937 gen(device_());
		std::uniform_int_distribution<_T> distribution(lower_bound, upper_bound);
		return distribution(gen);
	}

	[[nodiscard]] static short single(const short& lower_bound, const short& upper_bound) noexcept
	{
		return random<short>(lower_bound, upper_bound);
	}

	//[[nodiscard]] static std::chrono::milliseconds time(const short& lower_bound = 5000,
	//	const short& upper_bound = 15000) noexcept
	//{
	//	return random<std::chrono::milliseconds>(lower_bound, upper_bound);
	//}

private:
	static std::random_device device_;
}generator;

NO_EXPORT std::random_device Generator::device_;

class Entity {
	friend int checkInvalidPosition(Entity&, Entity&);
	friend int checkOutofBound(const Entity&);
	friend int checkSnakeFood(const Snake&, const Food&);
#pragma region Constructor
public:
	explicit Entity() : y(0), x(0), c(' '), nDirection(direction::NO_DIRECTION), speed(0) { count++; }
	explicit Entity(const Entity& entity) : y(entity.y), x(entity.x), c(entity.c), nDirection(entity.nDirection), speed(entity.speed) { count++; }
	explicit Entity(Entity&& entity) noexcept : y(entity.y), x(entity.x), c(entity.c), nDirection(entity.nDirection), speed(entity.speed) {
		entity.y = 0;
		entity.x = 0;
		entity.c = ' ';
		entity.nDirection = direction::NO_DIRECTION;
		entity.speed = 0;
		count++;
	}
	explicit Entity(const int& generateTag, const char& _c) : c(_c), speed(0) {
		switch (generateTag) {
		case GENERATE:
			y = generator.single(START_ROW + 1, START_ROW + BOX_HEIGHT - 1);
			x = generator.single(START_COL + 1, START_COL + BOX_WIDTH - 1);
			nDirection = generator.direct();
			break;
		case GENERATE_COOR:
			y = generator.single(START_ROW + 1, START_ROW + BOX_HEIGHT - 1);
			x = generator.single(START_COL + 1, START_COL + BOX_WIDTH - 1);
			nDirection = direction::NO_DIRECTION;
		case GENERATE_DIR:
			y = 0;
			x = 0;
			nDirection = generator.direct();
		default:
			[[unreachable]] exit(UNKNOWN_ERROR);
		}
		count++;
	}
	explicit Entity(const short& _y, const short& _x) : y(_y), x(_x), c(' '), nDirection(direction::NO_DIRECTION), speed(0) { count++; }
	explicit Entity(const short& _y, const short& _x, const char& _c) : y(_y), x(_x), c(_c), nDirection(direction::NO_DIRECTION), speed(0) { count++; }
	explicit Entity(const short& _y, const short& _x, const char& _c, const direction& _d) : y(_y), x(_x), c(_c), nDirection(_d), speed(0) { count++; }
	virtual ~Entity() = default;
#pragma endregion
public:
	// 'useful' function
	virtual Entity& show() noexcept {
		console
			.setCursorCoordinate(this->y, this->x)
			.print(std::string{ this->c });
		return *this;
	}
	virtual Entity& regenerate() noexcept {
		y = generator.single(START_ROW + 1, START_ROW + BOX_HEIGHT - 1);
		x = generator.single(START_COL + 1, START_COL + BOX_WIDTH - 2);
		return *this;
	}
	virtual Entity& move(const short& offset = 1) noexcept
	{
		console
			.setCursorCoordinate(y, x)
			.print(" ");
		switch (nDirection)
		{
		case direction::UP:
			y -= static_cast<short>(offset * speed);
			break;
		case direction::DOWN:
			y += static_cast<short>(offset * speed);
			break;
		case direction::LEFT:
			x -= static_cast<short>(offset * speed);
			break;
		case direction::RIGHT:
			x += static_cast<short>(offset * speed);
			break;
		default:
			[[unlikely]];
			//exit(INVALID_DIRECTION_INPUT); // sth wrong here
		}
		console
			.setCursorCoordinate(y, x)
			.print(std::string{ c });
		return *this;
	}
	// probably 'useless' function, I thought.
	[[nodiscard]] virtual constexpr char getChar() const noexcept final {
		return c;
	}
#pragma region 'get' Function
public:
	[[nodiscard]] virtual constexpr short getY() const noexcept final {
		return y;
	}
	[[nodiscard]] virtual constexpr short getX() const noexcept final {
		return x;
	}
	[[nodiscard]] virtual constexpr direction getDirection() const noexcept final {
		return nDirection;
	}
	[[nodiscard]] virtual constexpr double getSpeed() const noexcept final {
		return speed;
	}
#pragma endregion
public:
	// I think it's ok to make the member variable public
	short y;
	short x;
	char c;
	direction nDirection;
	double speed;
	static unsigned short count;
};

NO_EXPORT unsigned short Entity::count = 0;

class Food final : public Entity {
public:
	// as now food is simple, I don't think it's necessary to have lots of ctor
	explicit Food(const char& _c) : Entity(GENERATE, _c) {
		Entity::show();
	}
	// (temporary) disable the move function for food
	Food& move([[maybe_unused]] const short& offset = 1) noexcept override {
		return *this;
	}
	virtual ~Food() noexcept override {}
};

class Node final : public Entity {
public:
	explicit Node(const short& _y, const short& _x, const direction& _d) : Entity(_y, _x, 'o', _d) {}
	virtual ~Node() noexcept override {}
public:
	Node& move(const short& offset = 1) noexcept override {
		Entity::move(offset);
		return *this;
	}
};

class Snake final : public Entity {
public:
	explicit Snake() : Entity(), length(1), score(0) {
		speed = 1;
	}
	explicit Snake(const char& _c) :
		Entity(GENERATE, _c),
		length(1),
		score(0) {
		speed = 1;
		// check snake direction and put the following node
		for (short i = 1; i < generator.single(3, 5); i++)
			nodes.emplace_back(Node(y + i, x, nDirection));
		this->Snake::show();
	}
	virtual ~Snake() noexcept override {}
public:
	// the node of the snake
	std::vector<Node> nodes;
	short length;
	short score;
public:
	virtual Snake& show() noexcept override {
		Entity::show();
		for (auto& node : nodes) {
			node.show();
		}
		return *this;
	};
public:
	Snake& grow(const short& offset = 1) noexcept {
		// TODO: add a node, and set the direction of the node
		// increase length and score
		// occurs when snake eat food
		for (short i = 0; i < offset; i++) {
			// TODO: implement the growth function
		}
		return *this;
	}
	virtual Snake& move(const short& offset = 1) noexcept override {
		Entity::move(offset);
		// TODO: implement body movement

		// debug
		console.setCursorCoordinate(0, 0);
		console.top("You pressed a key.")
			.setCursorCoordinate(1, console.cursorCol / 3);
		std::print(" {} keys in total.", time++);
		console.setCursorCoordinate(y, x);
		return *this;
	}
	//inline Snake& changeDirection(const direction& _d) noexcept;
};

NO_EXPORT DEFINITION inline int checkOutofBound(const Entity& entity) {
	// as for snake, I think only to check the head is enough
	if (entity.y < START_ROW + 1 ||
		entity.y > START_ROW + BOX_HEIGHT - 1 ||
		entity.x < START_COL + 1 ||
		entity.x > START_COL + BOX_WIDTH - 2) {
		return INVALID;
	}
	return VALID;
}
NO_EXPORT DEFINITION int checkSnakeFood(const Snake& snake, const Food& food) {
	if (snake.y == food.y && snake.x == food.x) {
		// TODO: implement the snake body check
		return INVALID;
	}
	return VALID;
}

// we can use 'static_cast' when we have the guarantee that the cast is safe and valid,
//			otherwise, it may lead to undefined behavior.
// In that case, we should use 'dynamic_cast' instead.
NO_EXPORT DEFINITION int checkInvalidPosition(Entity& entity1, Entity& entity2) {
	if (entity1.y == entity2.y && entity1.x == entity2.x) {
		return INVALID;
	}
	if (instanceof<Entity, Snake>(&entity1) && instanceof<Entity, Food>(&entity2)) {
		return checkSnakeFood(static_cast<Snake&>(entity1), static_cast<Food&>(entity2));
	}
	if (instanceof<Entity, Food>(&entity1) && instanceof<Entity, Snake>(&entity2)) {
		return checkSnakeFood(static_cast<Snake&>(entity2), static_cast<Food&>(entity1));
	}
	return VALID;
}

namespace game
{
NO_EXPORT DEFINITION void clock(const std::chrono::seconds& cur, const int& row = TIMER_ROW, const int& col = TIMER_COL)
{
	console
		.printAndReturn(TIMER_ROW, TIMER_COL, std::to_string(cur.count()));
}

NO_EXPORT DEFINITION int gameOver()
{
	api::soundEvent(L"\\Media\\Windows Critical Stop.wav");
	console
		.bot("Game Over!", ansiColor::red, ansiBackground::black)
		.centeredAndReturn(console.height - 2, "Press Enter to return to menu", ansiColor::white, ansiBackground::black);
	game::elapsed = std::chrono::milliseconds(0);
	console.setCursorCoordinate(0, 0);
	std::cin.get();
	system("CLS");
	return GAMEOVER;
}
NO_EXPORT DEFINITION void gameInit()
{
	system("CLS");
	console.box(START_ROW, START_COL, BOX_HEIGHT, BOX_WIDTH)
		.box(TIMER_ROW, TIMER_COL, TIMER_HEIGHT, TIMER_WIDTH);
	api::soundEvent(L"\\Media\\Ring01.wav");
	for (int i = 0; i < 3; i++) {
		console.bot("Game will start in " + std::to_string(3 - i) + " seconds", ansiColor::green, ansiBackground::black);
		std::this_thread::sleep_for(std::chrono::milliseconds(1000));
	}
	console.bot("Game Start!", ansiColor::green, ansiBackground::black);
}
DEFINITION int snakeGame() {
	Snake snake('@');
	Food food('$');
	game::gameInit();
	bool isIgnore = false;
	int prevKey = -2;
	while (true) {
		if (console.getch(true)) {
			const int ch = console.getch();
			// alpha 224
			if (ch == 224 || ch == prevKey) continue;
			prevKey = ch;
			switch (ch) {
			case KEY_UP:
				if (snake.nDirection == direction::DOWN || snake.nDirection == direction::UP) {
					// ignore the key
					isIgnore = true;
					break;
				}
				snake.nDirection = direction::UP;
				break;
			case KEY_DOWN:
				if (snake.nDirection == direction::UP || snake.nDirection == direction::DOWN) {
					// ignore the key
					isIgnore = true;
					break;
				}
				snake.nDirection = direction::DOWN;
				break;
			case KEY_LEFT:
				if (snake.nDirection == direction::RIGHT || snake.nDirection == direction::LEFT) {
					// ignore the key
					isIgnore = true;
					break;
				}
				snake.nDirection = direction::LEFT;
				break;
			case KEY_RIGHT:
				if (snake.nDirection == direction::LEFT || snake.nDirection == direction::RIGHT) {
					// ignore the key
					isIgnore = true;
					break;
				}
				snake.nDirection = direction::RIGHT;
				break;
			default:
				isIgnore = true;
				break;
			}
		}
		snake.move();
		food.show();
		if (checkOutofBound(snake) == INVALID) {
			return gameOver();
		}
		if (checkInvalidPosition(snake, food) == INVALID)
		{
			food.regenerate().show();
			// TODO: implement the `grow` function
			//snake.grow();
			api::soundEvent(L"\\Media\\Windows Proximity Notification.wav");
		}
		if (isIgnore) {
			isIgnore = false;
		}
		else
		{
			game::timer();
		}
	}
	[[unreachable]] exit(UNKNOWN_ERROR);
}
}