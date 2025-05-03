#include "BlockManager.hpp"
#include <iostream>

BlockManager::BlockManager(sf::RenderWindow* window, Car* car, sf::View* blocksView):
	window(window), car(car), blocksView(blocksView)
{
	blocksBackground.setFillColor(sf::Color(200, 200, 200));
	blocksBackground.setSize({ 1920, 1080 });
	storeBackground.setFillColor(sf::Color(150, 150, 150));
	storeBackground.setSize({ blockStoreWidth, 1080 });

	blockStore.push_back(new StartBlock(sf::Vector2f(0, 0), window, blocksView));
	blockStore.push_back(new TimerBlock(sf::Vector2f(0, 0), window, blocksView));
	blockStore.push_back(new AccelerationBlock(sf::Vector2f(0, 0), window));
	blockStore.push_back(new DecelerationBlock(sf::Vector2f(0, 0), window));
	blockStore.push_back(new RotationBlock(sf::Vector2f(0, 0), window, blocksView, sf::degrees(15)));
	blockStore.push_back(new RotationBlock(sf::Vector2f(0, 0), window, blocksView, sf::degrees(-15)));
	blockStore.push_back(new MoveToBlock(sf::Vector2f(0, 0), window, blocksView));
	blockStore.push_back(new ConstSpeedBlock(sf::Vector2f(0, 0), window, blocksView));

	float y = 10;
	for (auto block : blockStore) {
		block->moveTo({ 10, y });
		y += block->size.y + 10;
	}

	EventBus::get().subscribe<sf::Event::MouseButtonPressed>(this, &BlockManager::onMouseButtonPressed);
	EventBus::get().subscribe<sf::Event::MouseButtonReleased>(this, &BlockManager::onMouseButtonReleased);
	EventBus::get().subscribe<sf::Event::MouseMoved>(this, &BlockManager::onMouseMoved);
	EventBus::get().subscribe<ClearBlocksEvent>(this, &BlockManager::onClearBlocks);
}

void BlockManager::update(sf::Time deltaTime)
{
	if (nextBlockToUpdate && isRunning) {
		nextBlockToUpdate->update(*car, deltaTime);
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

void BlockManager::saveToFile(std::wstring fileName)
{
	std::ofstream file(fileName);
	for (const auto& block : blocks) {
		if (block->name() == "RotationBlock") {
			RotationBlock* rb = dynamic_cast<RotationBlock*>(block);
			file << rb->name() << "\t" << rb->pos.x << "\t" << rb->pos.y << "\t" << rb->getAngle() << "\n";
		}
		else if (block->name() == "TimerBlock") {
			TimerBlock* tb = dynamic_cast<TimerBlock*>(block);
			file << tb->name() << "\t" << tb->pos.x << "\t" << tb->pos.y << "\t" << tb->size.y << "\t" << tb->getDuration() << "\n";
		}
		else if (block->name() == "MoveToBlock") {
			MoveToBlock* mtb = dynamic_cast<MoveToBlock*>(block);
			file << mtb->name() << "\t" << mtb->pos.x << "\t" << mtb->pos.y << "\t" << mtb->getTargetPos().x << "\t" << mtb->getTargetPos().y << "\n";
		}
		else if (block->name() == "ConstSpeedBlock") {
			ConstSpeedBlock* csb = dynamic_cast<ConstSpeedBlock*>(block);
			file << csb->name() << "\t" << csb->pos.x << "\t" << csb->pos.y << "\t" << csb->getTargetSpeed() << "\t" << "\n";
		}
		else {
			file << block->name() << "\t" << block->pos.x << "\t" << block->pos.y << "\n";
		}
	}
	file.close();
	isSavedNow = true;
}

void BlockManager::loadFromFile(std::wstring fileName)
{
	std::ifstream file(fileName);
	if (!file) {
		return;
	}
	blocks.clear();
	while (!file.eof()) {
		std::string type;
		sf::Vector2f pos;
		file >> type >> pos.x >> pos.y;
		Block* block = nullptr;
		if (type == "StartBlock") {
			block = new StartBlock(pos, window, blocksView);
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

			block = new TimerBlock(pos, window, blocksView, height, duration);
		}
		else if (type == "RotationBlock") {
			float angle;
			file >> angle;
			block = new RotationBlock(pos, window, blocksView, sf::degrees(angle));
		}
		else if (type == "MoveToBlock") {
			sf::Vector2f targetPos;
			file >> targetPos.x >> targetPos.y;
			block = new MoveToBlock(pos, window, blocksView, targetPos);
		}
		else if (type == "ConstSpeedBlock") {
			float targetSpeed;
			file >> targetSpeed;
			block = new ConstSpeedBlock(pos, window, blocksView, targetSpeed);
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
	leftHold = false;
}

bool BlockManager::isSaved() const
{
	return isSavedNow;
}

void BlockManager::start(StartBlock* sb)
{
	if (!isRunning) {
		isRunning = true;
		nextBlockToUpdate = sb->getNext();
	}
}

void BlockManager::onMouseButtonPressed(const sf::Event::MouseButtonPressed& mouseButtonPressed) {
	if (mouseButtonPressed.button == sf::Mouse::Button::Left) {
		sf::FloatRect blocksViewRect({
			(*blocksView).getCenter().x - (*blocksView).getSize().x / 2,
			(*blocksView).getCenter().y - (*blocksView).getSize().y / 2 },
			{ (*blocksView).getSize().x, (*blocksView).getSize().y });
		sf::Vector2f worldPos = window->mapPixelToCoords(mouseButtonPressed.position, *blocksView);

		if (blocksViewRect.contains(sf::Vector2f(mouseButtonPressed.position))) {
			if (worldPos.x > blockStoreWidth) {
				EventBus::get().publish(BlockPressedEvent{
					worldPos,
					sf::Mouse::Button::Left,
					isRunning
					});
			}			

			startPos = worldPos;
			bool onBlock = false;
			for (auto& block : blocks) {
				if (block->isInBoundingBox(worldPos)) {
					onBlock = true;
					if(!isRunning)
						movingBlock = block;
				}
			}

			if (!movingBlock && worldPos.x > blockStoreWidth && !onBlock) {
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
			isSavedNow = false;
		}
	}
}

void BlockManager::onMouseButtonReleased(const sf::Event::MouseButtonReleased& mouseButtonReleased) {
	leftHold = false;
	if (mouseButtonReleased.button == sf::Mouse::Button::Left && !isRunning) {		
		for (auto iter = blocks.begin(); iter != blocks.end();) {
			Block* block = *iter;

			if (block->pos.x < blockStoreWidth) {
				iter = blocks.erase(iter);
				delete block;
				isSavedNow = false;
				continue;
			}
			else {
				iter++;
			}
			if (block->blockInteract(movingBlock)) {
				isSavedNow = false;
				break;
			}
		}

		movingBlock = nullptr;
	}
}

void BlockManager::onMouseMoved(const sf::Event::MouseMoved& mouseMoved) {
	sf::Vector2f worldPos = window->mapPixelToCoords(mouseMoved.position, *blocksView);
	sf::FloatRect viewBounds({
			(*blocksView).getCenter().x - (*blocksView).getSize().x / 2,
			(*blocksView).getCenter().y - (*blocksView).getSize().y / 2 },
		{ (*blocksView).getSize().x, (*blocksView).getSize().y });
	if (viewBounds.contains(worldPos)) {
		if (!isRunning) {
			if (movingBlock != nullptr) {
				movingBlock->moveBy(worldPos - startPos);
				startPos = worldPos;
				isSavedNow = false;
			}
		}
		if (leftHold) {
			for (Block* block : blocks) {
				sf::Vector2f delta = worldPos - startPos;
				if (block->prevBlock == nullptr && block->pos.x + delta.x > blockStoreWidth) {
					block->moveBy(delta);
				}
			}
			startPos = worldPos;
			isSavedNow = false;
		}
	}
}

void BlockManager::onClearBlocks(const ClearBlocksEvent& event)
{
	blocks.clear();
	EventBus::get().publish<StopSimulationEvent>(StopSimulationEvent{});
	isSavedNow = false;
}
