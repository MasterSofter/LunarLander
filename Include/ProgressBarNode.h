#ifndef LUNARLANDER_PROGRESSBARNODE_H
#define LUNARLANDER_PROGRESSBARNODE_H


#include "SceneNode.h"
#include "ProgressBar.h"
#include "World.h"
#include <utility>

class ProgressBarNode : public SceneNode {
    public:
        typedef std::unique_ptr<ProgressBarNode> Ptr;

        explicit ProgressBarNode(World& world, float maxValue, sf::Vector2f size, float angle = 0.0, bool labelLeft = false);
        sf::Color getColor() const { return _progressColor; }

        void setValue(float value);
        void setLabelSide(TextNode::Alignment alignment);
        void setLabelPersent(bool value);

    private:
        virtual void drawCurrent(sf::RenderTarget &target, sf::RenderStates states) const;

    private:
        World& _world;
        ProgressBar _progressBar;
        TextNode* _valueLabel;

        sf::Vector2f _padding;
        sf::Color _progressColor;
        float _maxValue;
        bool _labelPersent;
};

#endif //LUNARLANDER_PROGRESSBARNODE_H
