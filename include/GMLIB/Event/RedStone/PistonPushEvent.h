#pragma once
#include "GMLIB/Macros.h"
#include "ll/api/event/world/WorldEvent.h"

namespace GMLIB::Event::RedStoneEvent {

class PistonPushBeforeEvent final : public ll::event::Cancellable<ll::event::WorldEvent> {
protected:
    BlockPos const& mBlockPos;
    uchar const&    mCurrentBranchFacing;
    uchar const&    mPistonMoveFacing;
    bool const&     mIsRetracting;

public:
    constexpr explicit PistonPushBeforeEvent(
        BlockSource&    region,
        BlockPos const& currentPos,
        uchar const&    currentBranchFacing,
        uchar const&    pistonMoveFacing,
        bool const&     isRetracting
    )
    : Cancellable(region),
      mBlockPos(currentPos),
      mCurrentBranchFacing(currentBranchFacing),
      mPistonMoveFacing(pistonMoveFacing),
      mIsRetracting(isRetracting) {}

    GMLIB_API BlockPos const& getPosition() const;
    GMLIB_API uchar const&    getBranchFacing() const;
    GMLIB_API uchar const&    getPistonMoveFacing() const;
    GMLIB_API bool const&     isRetracting() const;
};

class PistonPushAfterEvent final : public ll::event::WorldEvent {
protected:
    BlockPos const& mBlockPos;
    uchar const&    mCurrentBranchFacing;
    uchar const&    mPistonMoveFacing;
    bool const&     mIsRetracting;

public:
    constexpr explicit PistonPushAfterEvent(
        BlockSource&    region,
        BlockPos const& currentPos,
        uchar const&    currentBranchFacing,
        uchar const&    pistonMoveFacing,
        bool const&     isRetracting
    )
    : WorldEvent(region),
      mBlockPos(currentPos),
      mCurrentBranchFacing(currentBranchFacing),
      mPistonMoveFacing(pistonMoveFacing),
      mIsRetracting(isRetracting) {}

    GMLIB_API BlockPos const& getPosition() const;
    GMLIB_API uchar const&    getBranchFacing() const;
    GMLIB_API uchar const&    getPistonMoveFacing() const;
    GMLIB_API bool const&     isRetracting() const;
};

class BlockMovedByPistonAfterEvent final : public ll::event::world::WorldEvent {
protected:
    BlockPos&    mMovedBlockPos; // 被推动方块坐标
    Block const& mBlock;         // 被推动方块

public:
    constexpr explicit BlockMovedByPistonAfterEvent(BlockPos& MovedBlockPos, Block& block, BlockSource& BlockSource)
    : mMovedBlockPos(MovedBlockPos),
      mBlock(block),
      WorldEvent(BlockSource) {}

    GMLIB_API BlockPos&    getPosition() const;
    GMLIB_API Block const& getBlock() const;
};

} // namespace GMLIB::Event::RedStoneEvent