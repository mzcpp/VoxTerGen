#ifndef SETTINGS_HPP
#define SETTINGS_HPP

class Settings
{
private:
	int chunk_radius_;
public:
	Settings();

	int ChunkRadius() { return chunk_radius_; }

	void SetChunkRadius(int chunk_radius) { chunk_radius_ = chunk_radius; }
};

#endif

