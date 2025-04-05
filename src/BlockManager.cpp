#include "BlockManager.hpp"

BlockManager::BlockManager(sf::RenderWindow* window, Car* car, sf::View* blocksView):
	window(window), car(car), blocksView(blocksView)
{
	blocksBackground.setFillColor(sf::Color(200, 200, 200));
	blocksBackground.setSize({ 1920, 1080 });
	storeBackground.setFillColor(sf::Color(150, 150, 150));
	storeBackground.setSize({ blockStoreWidth, 1080 });

	blockStore.push_back(new StartBlock(sf::Vector2f(0, 0), window));
	blockStore.push_back(new TimerBlock(sf::Vector2f(0, 0), window));
	blockStore.push_back(new AccelerationBlock(sf::Vector2f(0, 0), window));
	blockStore.push_back(new DecelerationBlock(sf::Vector2f(0, 0), window));
	blockStore.push_back(new RotationBlock(sf::Vector2f(0, 0), window, sf::degrees(5)));
	blockStore.push_back(new RotationBlock(sf::Vector2f(0, 0), window, sf::degrees(-5)));

	float y = 10;
	for (auto block : blockStore) {
		block->moveTo({ 10, y });
		y += block->size.y + 10;
	}

	loadFromFile();

	EventBus::get().subscribe<StartSimulationEvent>(this, &BlockManager::onSimulationStart);
	EventBus::get().subscribe<sf::Event::MouseButtonPressed>(this, &BlockManager::onMouseButtonPressed);
	EventBus::get().subscribe<sf::Event::MouseButtonReleased>(this, &BlockManager::onMouseButtonReleased);
	EventBus::get().subscribe<sf::Event::MouseMoved>(this, &BlockManager::onMouseMoved);
}

void BlockManager::update(sf::Time deltaTime)
{
	elapsed += deltaTime;

	if (nextBlockToUpdate) {
		elapsed -= nextBlockToUpdate->update(*car, elapsed);
		if (!nextBlockToUpdate->isRunning) {
			nextBlockToUpdate = nextBlockToUpdate->getNext();
		}
	}
}

void BlockManager::render()
{
	window->draw(blocksBackground);
	window->draw(storeBackground);

	for (const auto& block : blockStore) {
		block->render();
	}
	for (const auto& block : blocks) {
		block->render();
	}
}

void BlockManager::saveToFile()
{
	std::ofstream file("save.dat");
	for (const auto& block : blocks) {
		if (block->name() == "RotationBlock") {
			RotationBlock* rb = dynamic_cast<RotationBlock*>(block);
			file << rb->name() << "\t" << rb->pos.x << "\t" << rb->pos.y << "\t" << rb->getAngle() << "\n";
		}
		else if (block->name() == "TimerBlock") {
			TimerBlock* tb = dynamic_cast<TimerBlock*>(block);
			file << tb->name() << "\t" << tb->pos.x << "\t" << tb->pos.y << "\t" << tb->size.y << "\t" << tb->getDuration() << "\n";
		}
		else {
			file << block->name() << "\t" << block->pos.x << "\t" << block->pos.y << "\n";
		}
	}
	file.close();
}

void BlockManager::loadFromFile()
{
	std::ifstream file("save.dat");
	if (!file) {
		return;
	}
	while (!file.eof()) {
		std::string type;
		sf::Vector2f pos;
		file >> type >> pos.x >> pos.y;
		Block* block = nullptr;
		if (type == "StartBlock") {
			block = new StartBlock(pos, window);
		}
		else if (type == "AccelerationBlock") {
			block = new AccelerationBlock(pos, window);
		}
		else if (type == "DecelerationBlock") {
			block = new DecelerationBlock(pos, window);
		}
		else if (type == "TimerBlock") {
			double duration;
			float height;
			file >> height >> duration;

			block = new TimerBlock(pos, window, height, duration);
		}
		else if (type == "RotationBlock") {
			float angle;
			file >> angle;
			block = new RotationBlock(pos, window, sf::degrees(angle));
		}
		if (block) {
			blocks.push_back(block);
		}
	}
	file.close();
	for (Block* a : blocks) {
		for (Block* b : blocks) {
			a->blockInteract(b, false);
		}
	}
}

void BlockManager::reset()
{
	if (isRunning) {
		isRunning = false;
		EventBus::get().publish<DisableBlocksEvent>(DisableBlocksEvent{});
		nextBlockToUpdate = nullptr;
		for (const auto& block : blocks) {
			block->deactivate(*car);
		}
	}	
}

void BlockManager::onSimulationStart(const StartSimulationEvent& event)
{
	if (!isRunning) {
		isRunning = true;
		nextBlockToUpdate = event.startBlock->getNext();
		elapsed = sf::seconds(0);
	}
}

void BlockManager::onMouseButtonPressed(const sf::Event::MouseButtonPressed& mouseButtonPressed) {
	if (mouseButtonPressed.button == sf::Mouse::Button::Left) {
		sf::FloatRect blocksViewRect(window->getView().getCenter() - window->getView().getSize() / 2.f, window->getView().getSize());
		sf::Vector2f worldPos = window->mapPixelToCoords(mouseButtonPressed.position, *blocksView);

		if (blocksViewRect.contains(sf::Vector2f(mouseButtonPressed.position))) {
			EventBus::get().publish(BlockPressedEvent{
					worldPos,
					sf::Mouse::Button::Left,
					isRunning
				});
		}

		startPos = worldPos;
		for (auto& block : blocks) {
			if (block->isInBoundingBox(worldPos) && !isRunning) {
				movingBlock = block;
			}
		}

		if (!movingBlock && blocksViewRect.contains(sf::Vector2f(mouseButtonPressed.position)) && worldPos.x > blockStoreWidth && !isRunning) {
			leftHold = true;
		}

		if (!isRunning) {
			for (auto& block : blockStore) {
				if (block->isInBoundingBox(worldPos)) {
					startPos = worldPos;
					movingBlock = block->clone();
					blocks.push_back(movingBlock);
				}
			}
		}
	}
}

void BlockManager::onMouseButtonReleased(const sf::Event::MouseButtonReleased& mouseButtonReleased) {
	if (mouseButtonReleased.button == sf::Mouse::Button::Left && !isRunning) {
		leftHold = false;
		for (auto iter = blocks.begin(); iter != blocks.end();) {
			Block* block = *iter;

			if (block->pos.x + block->size.x < blockStoreWidth) {
				iter = blocks.erase(iter);
				delete block;
				continue;
			}
			else {
				iter++;
			}
			if (block->blockInteract(movingBlock)) {
				break;
			}
		}

		movingBlock = nullptr;
	}
}

void BlockManager::onMouseMoved(const sf::Event::MouseMoved& mouseMoved) {
	sf::Vector2f worldPos = window->mapPixelToCoords(mouseMoved.position, *blocksView);
	bool isOver = false;
	for (Block* block : blocks) {
		if (block->name() == "StartBlock") {
			StartBlock* sb = dynamic_cast<StartBlock*>(block);
			if (sb->isMouseOver(worldPos)) {
				isOver = true;
				break;
			}
		}
	}
	if (isOver) {
		const auto cursor = sf::Cursor::createFromSystem(sf::Cursor::Type::Hand).value();
		window->setMouseCursor(cursor);
	}
	else {
		const auto cursor = sf::Cursor::createFromSystem(sf::Cursor::Type::Arrow).value();
		window->setMouseCursor(cursor);
	}

	if (!isRunning) {
		if (movingBlock != nullptr) {
			sf::FloatRect viewBounds({
				(*blocksView).getCenter().x - (*blocksView).getSize().x / 2,
				(*blocksView).getCenter().y - (*blocksView).getSize().y / 2 },
				{ (*blocksView).getSize().x, (*blocksView).getSize().y });


			if (viewBounds.contains(worldPos)) {
				movingBlock->moveBy(worldPos - startPos);
				startPos = worldPos;
			}
		}
		else if (leftHold) {
			for (Block* block : blocks) {
				block->pos += worldPos - startPos;
				if (block->pos.x < blockStoreWidth) {
					block->pos.x = blockStoreWidth;
				}
			}
			startPos = worldPos;
		}
	}
}