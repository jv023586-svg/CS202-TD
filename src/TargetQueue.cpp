/*
add()
  search
remove()
Node constructor()
  next
  prev
  head
  tail
  data
rangecheck()
*/

const float capRange = capsule.getAttackRange();
const float capRangeSq = capRange * capRange;

//It should take as arguments targetPriority and should return a pointer to an enemy.
//If targetPriority == Oldest, return the enemy that HEAD points to
//If targetPriority == Newest, return the enemy that tail points to
Enemy* getAttackTarget(/* TYPE */ targetPriority)
{
    if (/* targetPriority == Oldest */)
    {
        return targetQueue.front();
    } 
    else if(/* targetPriority == Newest */)
    {
        return targetQueue.back();
    } 
    
    static thread_local std::mt19937 rng(std::random_device{}()); // create a random number generator
    std::uniform_int_distribution<std::size_t> dist(0, alive.size() - 1); // create a uniform distribution for the size of the list of alive enemies
    attackTarget = alive[dist(rng)]; // set the attack target to a random enemy in the list of alive enemies

}

bool rangeCheck(Enemy* enemy /* absolute position */)
{
    const sf::Vector2f d = enemy.getPosition() - capPos;
    if (d.x * d.x + d.y * d.y <= capRangeSq) 
    {
        return true;
    }
    else 
    {
        return false
    } // pythagorean distance calculation to check if the enemy is in range of the capsule
}

bool checkIndex(int spawnIndex) 
{
    // If the spawn index of this enemy is less than or equal to the spawn index that Tail Node points to, return false. Else return true.
    if (spawnIndex <= targetQueue.back()) 
    {
        return false;
    }
    else
    {
        return true;
    }
}

void addEnemy(int spawnIndex)
{
    targetQueue.push_back(spawnIndex);
}

void removeEnemy(int spawnIndex, /* TYPE */ targetPriority)
{
    
}

void refreshTargetQueue()
{
    // for all enemy in enemies, call rangeCheck, call checkIndex(), call isAlive(). All three must be true to enter the loop.
    // Inside the loop, call addEnemy    
    for (int i = 0; i < enemies.size(); i++)
    {
        if (enemies[i].isAlive())
        {
            if (rangeCheck(enemies[i]) && checkIndex(i+1))
            {
                addEnemy(i+1);
            }
        }
    }
}



