template <typename TextureAdaptor>
FontTextureCache::FontTextureCache(TextureAdaptor textureAdaptor):
    mGlyphPacker(32),
    mCurrentSize(32)
{
    std::shared_ptr<TextureAdaptor> textureAdaptorToKeep = std::make_shared<TextureAdaptor>(textureAdaptor);
    textureAdaptorToKeep->initialize(32, 32);

    mResizeStorage = [=] (uint32_t width, uint32_t height)
    {
        textureAdaptorToKeep->resize(width, height);
    };

    mWriteBitmap = [=] (uint32_t width, uint32_t height, const BitMap& bitMap)
    {
        textureAdaptorToKeep->writeBitmap(width, height, bitMap);
    };
}
