// edge detection
/*
  Edge MyEdge = Edge();
  if (MyEdge.rising(digitalRead(0)))
  { }
*/
class Edge
{
  public:
    Edge()
    {
      _boRising = true;
    }

    Edge(bool rising)
    {
      _boRising = rising;
    }

    void Init(bool state) {
      _boOldState = state;
    }

    bool Rising(bool state)
    {
      if (state != _boOldState)
      {
        _boOldState = state;

        if (state == true)
        {
          return true;
        }
      }

      return false;
    }

    bool Falling(bool state)
    {
      if (state != _boOldState)
      {
        _boOldState = state;

        if (state == false)
        {
          return true;
        }
      }

      return false;
    }

    bool DetectEdge(bool state)
    {
      if (state != _boOldState)
      {
        _boOldState = state;
        return true;
      }

      return false;
    }

  private:
    bool _boRising;
    bool _boOldState = false;
};
