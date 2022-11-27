/*
  Timer MyTimer = Timer();
  MyTimer.Interval(1000);
  MyTimer.Start();
  if (MyTimer.elapsed())
  { }
*/
class Timer
{
  public:
    Timer()
    {
      Reset();
    }

    Timer(bool autoreset)
    {
      _boAutoReset = autoreset;

      Reset();
    }

    Timer(bool autoreset, bool usemicros)
    {
      _boAutoReset = autoreset;
      _boUseMicros = usemicros;

      Reset();
    }

    void Interval(unsigned long interval)
    {
      _ulInterval = interval;
    }

    void Start()
    {
      _boEnabled = true;
    }

    void Stop()
    {
      _boEnabled = false;
    }

    bool Enabled()
    {
      return _boEnabled;
    }

    void Reset()
    {
      _ulStartTime = _time();
    }

    bool Elapsed()
    {
      return Elapsed(_ulInterval);
    }

    bool Elapsed(unsigned long interval)
    {
      if (_boEnabled == false)
      {
        return false;
      }
      else
      {
        if ((_time() - _ulStartTime) < interval)
        {
          return false;
        }
        else
        {
          if (_boAutoReset)
          {
            Reset();
          }

          return true;
        }
      }
    }

    unsigned long ElapsedTime()
    {
      return _time() - _ulStartTime;
    }

  private:
    bool _boAutoReset = true;
    bool _boUseMicros = false;
    bool _boEnabled = false;
    unsigned long _ulInterval;
    unsigned long _ulStartTime;

    unsigned long _time()
    {
      if (_boUseMicros)
      {
        return micros();
      }
      else
      {
        return millis();
      }
    }
};