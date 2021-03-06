#include "GameObject.h"
#include <mmsystem.h>
#include "DuckObject.h"


//create new objects & setup it

GameObject::GameObject()
{
  road_ = new BaseDrawElement[Application::instance().getHeight()];
  //generate road mass in hight of screen of game
  for (int i = 0; i < Application::instance().getHeight(); i++)
  {
    road_[i].set(
      Application::instance().getWidth() * 0.25, i,
      Application::instance().getWidth() * 0.5,  1
    );
  }

  //creation car
  car_ = BaseDrawElement(Application::instance().getWidth() * 0.5, Application::instance().getHeight() - 6, 4, 5);
  
  //duckCrossingRoad_ = BaseDrawElement(Application::instance().getWidth()*0.1, Application::instance().getHeight() / 2, 4, 5);
  
  bush_ = BaseDrawElement(Application::instance().getWidth()*0.2, Application::instance().getHeight()* 0.2, 4, 5);
  //creation mass of bariers
  barriers_[0] = BaseDrawElement(random(road_[0].x, road_[0].getRight()),
                      random(-Application::instance().getHeight() * 0.7, -3), 4, 2);
  barriers_[1] = BaseDrawElement(random(road_[0].x, road_[0].getRight()),
                      random(-Application::instance().getHeight() * 0.7, -3), 4, 2);
  barriers_[2] = BaseDrawElement(random(road_[0].x, road_[0].getRight()),
                      random(-Application::instance().getHeight() * 0.7, -3), 6, 3);

  //random set colors
  for (int i = 0; i < BARIER_COUNT; i++)
  {
    barrierColors[i] = random(COLOR_BLUE, COLOR_YELLOW);
  }
}

//Destructor (of course)
GameObject::~GameObject()
{
  delete[] road_;
}

//Update function
void GameObject::onUpdate()
{
  //control ride left, right
  if (toLeft)
  {
    car_.x -= 2;
  }
  if (toRight)
  {
    car_.x += 2;
  }

  //control speed
  if (speedUp)
  {
    speed_ += 0.1;
  }
  if (speedDown)
  {
    speed_ -= 0.15;
  }
  speed_ = range(0.1, speed_, 5.0);

  float prev = pos_;
  pos_ += speed_;

  int deltaY = int(pos_) - int(prev);

  for (int repeat = 0; repeat < deltaY; repeat++)
  {
    roadDirectionIterator_--;
    if (roadDirectionIterator_ < 0)
    {
      roadDirectionIterator_ = random(10, 100);
      roadDirection_ = roadDirection_ == 1 ? 0 : 1;
    }
    for (int i = Application::instance().getHeight() - 1; i > 0; i--)
    {
      road_[i].set(road_[i - 1].x, i, road_[i - 1].getWidth(), 1);
    }
    int x = road_[0].x + random(roadDirection_ ? -1 : 0, roadDirection_ ? 0 : 1);
    road_[0].x = range(Application::instance().getWidth() * 0.05, x, Application::instance().getWidth() * 0.45);
  }

  for (int i = 0; i < BARIER_COUNT; i++)
  {
    int y = barriers_[i].y + deltaY;
    if (y >= Application::instance().getWidth())
    {
      barrierColors[i] = random(COLOR_BLUE, COLOR_YELLOW);
      y = random(-20, -3);
      int x = road_[0].getWidth() / 2 + road_[0].x + random(-road_[0].getWidth() * 0.4, road_[0].getWidth() * 0.3);
      barriers_[i].x = x;
    }
    barriers_[i].y = y;
  }
  score_ += deltaY;
  pos_ = pos_ - int(pos_);

  //Collision calculate
  for (int i = 0; i < Application::instance().getWidth(); i++)
  {
    if (car_.isIntersectRect(BaseDrawElement(-1000, i, road_[i].x + 1000, 1)))
    {
      car_.x++;
    }
    if (car_.isIntersectRect(BaseDrawElement(road_[i].getRight(), i, 1000, 1)))
    {
      car_.x--;
    }
  }
  for (int i = 0; i < BARIER_COUNT; i++)
  {
    if (car_.isIntersectRect(barriers_[i]))
    {
      ///
      crush = true;
      
      //Application::instance().close();
    }
  }
}

//key handler press
void GameObject::onKeyDown(int key)
{
  switch (key)
  {
  case KEY_ARROW_UP:    speedUp   = true; break;
  case KEY_ARROW_DOWN:  speedDown = true; break;
  case KEY_ARROW_LEFT:  toLeft    = true; break;
  case KEY_ARROW_RIGHT: toRight   = true; break;
  default:                                break;
  }
}

//key handler depress 8)
void GameObject::onKeyUp(int key)
{
  switch (key)
  {
  case KEY_ARROW_UP:    speedUp    = false; break;
  case KEY_ARROW_DOWN:  speedDown  = false; break;
  case KEY_ARROW_LEFT:  toLeft     = false; break;
  case KEY_ARROW_RIGHT: toRight    = false; break;
  default:                                  break;
  }
}

int GameObject::getScore()
{
  return score_;
}



void GameObject::CanvasDrawPlayerCar(Canvas& canvas)
{
  canvas.clear(COLOR_RED - 8, car_);
  canvas.clear(COLOR_RED, BaseDrawElement(car_.x, car_.y + 2, 4, 2));
  canvas.drawRect(BaseDrawElement(car_.x + 1, car_.y, 2, 2), '|', COLOR_WHITE, COLOR_TRANSPARENT);
  canvas.drawCell(car_.x - 1, car_.y + 1, ' ', COLOR_BLACK, COLOR_BLACK);
  canvas.drawCell(car_.getRight(), car_.y + 1, ' ', COLOR_BLACK, COLOR_BLACK);
  canvas.drawCell(car_.x - 1, car_.y + 4, ' ', COLOR_BLACK, COLOR_BLACK);
  canvas.drawCell(car_.getRight(), car_.y + 4, ' ', COLOR_BLACK, COLOR_BLACK);
}



void GameObject::roadCreation(Canvas& canvas)
{
  for (int i = 0; i < Application::instance().getHeight(); i++)
  {
    canvas.clear(COLOR_GRAY, road_[i]);
    canvas.drawCell(road_[i].x, road_[i].y, ' ', COLOR_GRAY, COLOR_WHITE);
    canvas.drawCell(road_[i].getRight() - 1, road_[i].y, ' ', COLOR_GRAY, COLOR_WHITE);
  }
}

void GameObject::barriersCreation(Canvas& canvas)
{
  for (int i = 0; i < BARIER_COUNT; i++)
  {
    canvas.clear(barrierColors[i], barriers_[i]);
  }
}

//void GameObject::bushDrawing(Canvas& canvas)
//{
//  //x - L-R
//  //Y - up down
//  canvas.drawRect(BaseDrawElement(bush_.x, bush_.y, 8, 1), 178, COLOR_GREEN, COLOR_TRANSPARENT);
//  canvas.drawRect(BaseDrawElement(bush_.x, bush_.y - 1, 8, 1), 178, COLOR_GREEN, COLOR_TRANSPARENT);
//  canvas.drawRect(BaseDrawElement(bush_.x + 10, bush_.y, 8, 1), 178, COLOR_GREEN, COLOR_TRANSPARENT);
//  canvas.drawRect(BaseDrawElement(bush_.x + 9, bush_.y-1, 10, 1), 178, COLOR_GREEN, COLOR_TRANSPARENT);
//  canvas.drawRect(BaseDrawElement(bush_.x + 2, bush_.y -2, 9, 1), 178, COLOR_GREEN, COLOR_TRANSPARENT);
//}

void GameObject::onDraw(Canvas & canvas)
{
  //field
  canvas.clear(COLOR_DARKGREEN);
  //road
  roadCreation(canvas);
  //barrier
  barriersCreation(canvas);
  //Duck
  //duckDraw(canvas);
  //car
  CanvasDrawPlayerCar(canvas);
  //bush
 // bushDrawing(canvas);
  ///text
  canvas.drawText(Application::instance().getWidth() - 17, Application::instance().getHeight() - 3, "SCORE:" + std::to_string(score_), COLOR_YELLOW, COLOR_BLACK);
  canvas.drawText(10, Application::instance().getHeight() - 3, "SPEED:" + std::to_string(int(speed_ * 10)), COLOR_YELLOW, COLOR_BLACK);
  
  if (!isEnabled())
  {
   
    canvas.drawText(Application::instance().getWidth() / 2 - 5,
                    Application::instance().getHeight()/2,
                    "PAUSED", COLOR_RED, COLOR_BLACK);
  }
  if (crush)
  {
    //PlaySound(TEXT("RickMorty.mp3"), nullptr, SND_ASYNC);
    BaseDrawElement rect;
    rect.set(0, 0, Application::instance().getWidth(), Application::instance().getHeight());
    canvas.drawRect(rect, '/', COLOR_RED, COLOR_RED);
    canvas.drawText(Application::instance().getWidth() / 2 - 5, Application::instance().getHeight() / 2, "Game over", COLOR_BLUE, COLOR_YELLOW);
  
    

  }
}
