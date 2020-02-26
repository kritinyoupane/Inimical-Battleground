#include "Player.h"

Player::Player():isPressedLeft(false),isPressedRight(false),isPressedUp(false),isCollided(false),isOnGround(false)
{
	
	/*isPressedLeft = false;
	isPressedRight = false;
	isPressedUp = false;
	isCollided = false;
	isOnGround = false;*/

	coordinate.x = 200;
	coordinate.y = 200;
	currentVelocity.x = 0;
	currentVelocity.y = 0;
	targetVelocity.x = 0;
	targetVelocity.y = 0;
	maxGroundVelocity.x =30;
	maxAirVelocity.x = 60;
	maxAirVelocity.y = 80;
	thrustValue = 100;

	texture.loadFromFile("pic/army1.png");

	body.setSize(sf::Vector2f(10.f, 10.f));
	//body.setRotation(60.f);

	groundDampingConstant = 100;
	airDampingConstant = 50;
	gravity = 50;
	elasticConstant = 0.3f;
	collider = (new Collider(body));
	//collider.body = this->body;

	noKeyWasPressed = true;
	//directions
	facingRight = true;
	facingLeft = false;
	//animations spritesheet
	animations[int(AnimationIndex::flyingright)] = Animation(0, 152, 95, 150, "right");
	animations[int(AnimationIndex::flyingleft)] = Animation(0, 152, 95, 150, "left");
	animations[int(AnimationIndex::walkingright)] = Animation(0, 0, 95, 150, "right");
	animations[int(AnimationIndex::walkingleft)] = Animation(0, 0, 95, 150, "left");
	animations[int(AnimationIndex::standingright)] = Animation(0, 0, 95, 150, "stillright");
	animations[int(AnimationIndex::standingleft)] = Animation(0, 167, 95, 150, "stillleft");
	lhand.loadFromFile("pic/rhandinverse.png");
	rhand.loadFromFile("pic/rhand.png");//animations[int(AnimationIndex::standing)] = Animation(0, 0, 95, 150, "left", noKeyWasPressed);
	
	playerHandLeft.setTexture(lhand);
	playerHandRight.setTexture(rhand);
	/*spritePlayer.setScale(0.5, 0.5);
	playerHandLeft.setScale(0.5, 0.5);
	playerHandRight.setScale(0.5,0.5);*/

}



void Player::update(sf::Time deltaTime)
{
	
	this->deltaTime = deltaTime;

	//std::cout << mousePosition.x << "," << mousePosition.y << "||"<<mouseDirection.x << "," << mouseDirection.y << std::endl;
	isOnGround = currentVelocity.y == 0;
	noKeyWasPressed = !(isPressedLeft || isPressedRight || isPressedUp);
	if (isOnGround) {
		movePlayer(maxGroundVelocity, groundDampingConstant);
		//std::cout << "Ground" << std::endl;
		}
	else {
		movePlayer(maxAirVelocity, airDampingConstant);
		//std::cout << "Air"<< std::endl;
	}
	if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && localTime.getElapsedTime().asSeconds()>0.4f) {
		if (facingLeft == false && facingRight == true) {
			bullets.emplace_back(Bullet(playerHandRight.getPosition(), mouseDirection, 2500, 50));
		}
		else {
			sf::Vector2f temp = playerHandLeft.getPosition();
			temp.x=playerHandLeft.getPosition().x ;
			bullets.emplace_back(Bullet(temp, mouseDirection, 2500, 50));
		}
		localTime.restart();

	}
		for (auto& bullet : bullets)
		{
			bullet.update(deltaTime);

		}
	

	

	handlePlayerCollision(bar);
	handlePlayerCollision(arena_blocks[0]);


	
}
void Player::setCoordinate(sf::Vector2f coordinate)
{
	this->coordinate = coordinate;
}
void Player::setCoordinate(float x, float y)
{
	sf::Vector2f temp;
	temp.x = x;
	temp.y = y;
	this->coordinate = temp;
}

void Player::getUserInput()
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
	{
		currentVelocity.x = -maxGroundVelocity.x;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
	{
		currentVelocity.x = maxGroundVelocity.x;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
	{
		currentVelocity.y = -maxGroundVelocity.y;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
	{
		currentVelocity.y = +maxGroundVelocity.y;
	}
}

Collider Player::getCollider()
{
	 return Collider(body); 
}



sf::Vector2f Player::getCoordinate()
{
	return this->coordinate;
}

sf::Vector2f Player::getVelocity()
{
	return this->currentVelocity;
}

float Player::interpolateVelocity(float target, float current, float dampingFactor)
{
	float increment = dampingFactor * this->deltaTime.asSeconds();
	float difference = target - current;
	if (difference >increment) {
		return current + increment;
	}
	if (difference < -increment) {
		return current - increment;
	}
	return target;

}

void Player::draw(sf::RenderWindow& window)
{
	for (auto& bullet : bullets)
	{
		bullet.draw(window);
	}

	if (facingLeft) {
		curAnimation = AnimationIndex::walkingleft;
		if (noKeyWasPressed)
		{
			curAnimation = AnimationIndex::standingleft;
		}
	}
	if (facingRight) {
		curAnimation = AnimationIndex::walkingright;
		if (noKeyWasPressed)
		{
			curAnimation = AnimationIndex::standingright;
		}
	}
	if (!isOnGround) {
		if (isPressedLeft || (isPressedUp && facingLeft)) {
			curAnimation = AnimationIndex::flyingleft;
		}
		if (isPressedRight || (isPressedUp && facingRight)) {
			curAnimation = AnimationIndex::flyingright;
		}
	}

	if (!(noKeyWasPressed)) {
		animations[int(curAnimation)].update(deltaTime);
	}
	animations[int(curAnimation)].applySprite(spritePlayer);
	body.setFillColor(sf::Color(100, 250, 50));
	body.setSize(sf::Vector2f(100, 150));
	window.draw(body);

	bar.setSize(sf::Vector2f(500, 50));
	bar.setFillColor(sf::Color(100, 250, 50));
	bar.setPosition(0, 500);
	window.draw(bar);

	arena_blocks[0].setFillColor(sf::Color(100, 250, 50));
	arena_blocks[0].setSize(sf::Vector2f(100, 50));
	arena_blocks[0].setPosition(900, 450);
	window.draw(arena_blocks[0]);


	setBulletDir(window);
	window.draw(spritePlayer);
	if (facingLeft)
	{
		/*playerHandLeft.setOrigin({
			playerHandLeft.getGlobalBounds().width / 2 + playerHandLeft.getOrigin().x,
			playerHandLeft.getGlobalBounds().height / 2 + playerHandLeft.getOrigin().y
			});*/


		playerHandLeft.setPosition(getCoordinate().x - 40 + playerHandLeft.getLocalBounds().width,
			getCoordinate().y + 60 + playerHandLeft.getLocalBounds().height);
		window.draw(playerHandLeft);
	}
	if (!facingLeft)
	{
		playerHandRight.setPosition(getCoordinate().x + 20, getCoordinate().y + 63);
		window.draw(playerHandRight);
	}


	spritePlayer.setPosition(getCoordinate().x, getCoordinate().y);
	


}


void Player::handlePlayerCollision(sf::RectangleShape other)
{
	if (collider->checkCollision((other))) {
		if (collider->intersection.height < collider->intersection.width) {
			if (body.getGlobalBounds().top < other.getGlobalBounds().top) {
				coordinate.y -= collider->intersection.height;
				if (abs(currentVelocity.y) < 1) { currentVelocity.y = 0; }
				currentVelocity.y = -elasticConstant * currentVelocity.y;
				targetVelocity.y = 0;
			}
			else {
				coordinate.y += collider->intersection.height;
				if (abs(currentVelocity.y) < 1) { currentVelocity.y = 0; }
				currentVelocity.y = -elasticConstant * currentVelocity.y;
				targetVelocity.y = currentVelocity.y;
			}
		}

		if (collider->intersection.height > collider->intersection.width) { //horizontal collission

			if (body.getGlobalBounds().left < other.getGlobalBounds().left) {
				coordinate.x -= collider->intersection.width;
				currentVelocity.x = -elasticConstant * currentVelocity.x;
				targetVelocity.x = currentVelocity.x;
			}
			else {
				coordinate.x += collider->intersection.width;
				currentVelocity.x = -elasticConstant * currentVelocity.x;
				targetVelocity.x = currentVelocity.x;
			}
		}
		body.setPosition(coordinate);


	}
}

void Player::movePlayer(sf::Vector2f maxVelocity, float dampingConstant)
{

	if (isPressedLeft) {
		targetVelocity.x = -maxVelocity.x;

		facingLeft = true;
		facingRight = false;
	}
	else if (isPressedRight) {
		targetVelocity.x = maxVelocity.x;
		facingRight = true;
		facingLeft = false;
	}
	else {
		targetVelocity.x = 0;
	}
	if (isPressedUp) {

		targetVelocity.y -= deltaTime.asSeconds() * thrustValue;

	}
	targetVelocity.y += deltaTime.asSeconds() * gravity;
	currentVelocity.x = interpolateVelocity(targetVelocity.x, currentVelocity.x, dampingConstant);
	currentVelocity.y = interpolateVelocity(targetVelocity.y, currentVelocity.y, dampingConstant);
	coordinate.x += currentVelocity.x * 5 * deltaTime.asSeconds();
	coordinate.y += currentVelocity.y * 5 * deltaTime.asSeconds();
	body.setPosition(coordinate);
}




void Player::setBulletDir(sf::RenderWindow& window)
{
	playerCenter = playerHandRight.getPosition();

	//playerHandRight.setPosition(getCoordinate().x + 10.f, getCoordinate().y + 20.f);

	mousePosition = sf::Vector2f(sf::Mouse::getPosition(window));
	mouseDirection = mousePosition - playerCenter;
	mouseDirection = mouseDirection / sqrt(pow(mouseDirection.x, 2) + pow(mouseDirection.y, 2));
	float pi = 3.14159265f;
	float deg = atan2(mouseDirection.y, mouseDirection.x) * 180 / pi;
	/*playerHandRight.setRotation(deg);
	if (deg < -90 || deg>90)
	{
		facingLeft = true;
		facingRight = false;
		playerHandLeft.setRotation(deg);
	}*/
	if (facingRight)
	{
		playerHandRight.setRotation(deg);
		if (deg < -90 || deg>90)
		{
			facingLeft = true;
			facingRight = false;
		}
	}
	if (facingLeft)
	{
		playerHandLeft.setRotation(deg);
		if (!(deg < -90 || deg>90))
		{
			facingLeft = false;
			facingRight = true;
		}
	}
}
