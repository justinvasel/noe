struct hit{
  int32_t cell, plane, adc, tdc;
  float tns;
  bool good_tns;
};

struct track{
  float startx, starty, startz;
  float stopx, stopy, stopz;
  std::vector<hit> hits;
};

struct noeevent{
  std::vector<hit> hits;
  std::vector<track> tracks;
  uint32_t nevent, nrun, nsubrun;

  // The first and last hits physically in the event
  int32_t mintick = 0x7fffffff, maxtick = 0;

  // The user-input minimum and maximum, which must be a subset of the
  // above. These are here so they can be remembered even if the user
  // navigates to another event and then back to this one.
  int32_t user_mintick = 0x7fffffff, user_maxtick = 0;

  // The actual range being displayed right now (or the last time the
  // event was drawn), which is a subset of the user_ times during an
  // animation. There is yet another relevant range of times, which
  // is the range that needs to be drawn. This is some subset of the
  // current_ times, depending on whether some hits are already on the
  // screen. Those don't make much sense to store with the event. At the
  // moment, the current_ times don't need to be stored with the event
  // either, since they are always reset upon switch to the event, but
  // it's easier to imagine that one might want to remember them so as
  // to be able to animate multiple events "simultaneously".
  //
  // Also as it stands, current_mintick is always either user_mintick or
  // current_maxtick, depending on whether the antimation is cumulative
  // or not.
  int32_t current_mintick = 0x7fffffff, current_maxtick = 0;

  bool fdlike = false;

  void addtrack(__attribute__((unused)) const track & t)
  {
    // This isn't well developed, so just don't.
    //tracks.push_back(t);
  }

  void addhit(const hit & h)
  {
    hits.push_back(h);

    // I don't want to use any art services, so autodetect when we are in
    // the FD.  This should fail very very rarely since the FD is noisy.
    if(!fdlike && (h.cell >= 3 * 32 || h.plane >= 8 * 24 + 22)) fdlike = true;

    if(h.tdc < mintick) current_mintick = user_mintick = mintick = h.tdc;
    if(h.tdc > maxtick) current_maxtick = user_maxtick = maxtick = h.tdc;
  }
};
