#ifndef CATS2_VIEWER_WIDGET_HPP
#define CATS2_VIEWER_WIDGET_HPP

#include "ViewerPointerTypes.hpp"

#include <AgentData.hpp>

#include <QtCore/QObject>
#include <QtWidgets/QWidget>
#include <QtGui/QPixmap>

class FrameScene;
class QGraphicsPixmapItem;
class QGraphicsTextItem;
class AgentText;
class AgentItem;

namespace Ui
{
class ViewerWidget;
}

/*!
 * \brief The user interface class that shows current video frame and the detected objects positions.
 */
class ViewerWidget : public QWidget
{
    Q_OBJECT
public:
    //! Constructor.
    explicit ViewerWidget(ViewerDataPtr data, QSize frameSize, QWidget *parent = nullptr);
    //! Destructor.
    virtual ~ViewerWidget() final;

signals:
    //! Notifies that the mouse position has changed and sends it out in both
    //! image and world coordinates.
    void mousePosition(PositionPixels imagePosition, PositionMeters worldPosition);
    //! Notifies that the right mouse button was clicked on the scene, and sends
    //! its position out in world coordinates.
    void notifyRightButtonClick(PositionMeters worldPosition);

public slots:
    //! Triggered on arrival of the new data.
    void showAgentLabels(QList<AgentDataWorld> agentsData);
    //! Triggered on arrival of the new data.
    void showAgents(QList<AgentDataWorld> agentsData);
    //! Set the flag that defines if the agents must be shown.
    void setShowAgents(bool agentsShown);

public slots:
    //! Zoom on the video.
    void onZoomIn();
    //! Unzoom the video.
    void onZoomOut();
    //! Save current frame.
    void saveCurrentFrameToFile();
    //! Scale to the available area.
    void adjust();

protected slots:
    //! A new frame arrived.
    void onNewFrame(QSharedPointer<QPixmap> pixmap, int fps);
    //! Triggered when a new mouse position is received from the scene.
    void onMouseMoved(QPointF scenePosition);
    //! Triggered when a right button click is received from the scene.
    void onRightButtonClicked(QPointF scenePosition);

protected:
    //! Computes new average frame rate value as exponential moving average.
    void updateFrameRate(int fps);
    //! Converts the scene position to PositionPixels and PositionMeters.
    bool convertScenePosition(const PositionPixels& imagePosition,
                              PositionMeters& worldPosition);

protected:
    //! The data object that provides the frames and agent's positions to show.
    ViewerDataPtr m_data;
    //! The frame size.
    QSize m_frameSize;
    //! The form.
    Ui::ViewerWidget* m_uiViewer;
    //! The graphics scene to show the video stream and agents.
    FrameScene* m_scene;
    //! The item used to show the video stream.
    QGraphicsPixmapItem* m_videoFrame;
    //! The item used to show the video's frame rate.
    QGraphicsTextItem* m_frameRate;
    //! The average frame rate.
    double m_averageFps;

    // FIXME : at the moment the agent's id is copied as it is without taking
    // into account that theoretically two agents coming from different sources
    // can have the same id.
    //! The map containing the agent's id's with the corresponding label
    //!  on the scene.
    QMap<QString, AgentText*> m_agentLabels;
    //! The map containing the agent's id's with the corresponding object
    //!  on the scene.
    QMap<QString, AgentItem*> m_agents;

    //! The flag that defines if we show agents on the map.
    bool m_agentsShown;
};

#endif // CATS2_VIEWER_WIDGET_HPP
