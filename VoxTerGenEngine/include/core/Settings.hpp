#ifndef SETTINGS_HPP
#define SETTINGS_HPP

class Settings
{
private:
	int chunk_radius_;
public:
	Settings();

	// Getters
	int ChunkRadius() { return chunk_radius_; }

	// Setters
	void SetChunkRadius(int chunk_radius) { chunk_radius_ = chunk_radius; }
};

#endif // SETTINGS_HPP

