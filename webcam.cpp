/****************************************************************************
 *  webcam
 *
 *  Copyright (c) 2012 by Nikita Belov <null@deltaz.org>
 *
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************
*****************************************************************************/

#include "webcam.h"
#include "winuser.h"

int n=0;

QByteArray webCam::m_defaultDevice = QByteArray(); 

webCam::webCam()
{
    ui.setupUi( this );

//	if ( QtWin::isCompositionEnabled() )
//	{
//		setAttribute( Qt::WA_TranslucentBackground, true );
//		QtWin::extendFrameIntoClientArea( this );
//	}

	QList< QByteArray > cams = QCamera::availableDevices();
	if ( !cams.contains( m_defaultDevice ) )
	{
		if ( cams.count() == 0 )
		{
			QMessageBox::critical( this, "Error", "Web Cams are not found!" );

			deleteLater();
			return;
		}
		else if ( cams.count() == 1 )
		{
			m_defaultDevice = cams.at( 0 );
		}
		else
		{
			m_selectDialog = new QDialog();
			m_selectDialog->setAttribute( Qt::WA_DeleteOnClose );
			m_selectDialog->setAttribute( Qt::WA_QuitOnClose, false );
			m_selectDialog->setWindowFlags( m_selectDialog->windowFlags() ^ Qt::WindowContextHelpButtonHint | Qt::MSWindowsFixedSizeDialogHint );

			select_ui.setupUi( m_selectDialog );

			foreach( QByteArray webCam, cams )
			{
				auto commandLinkButton = new QCommandLinkButton( QCamera::deviceDescription( webCam ) );
				commandLinkButton->setProperty( "webCam", webCam );

				connect( commandLinkButton, &QCommandLinkButton::clicked, [=]( bool )
					{
						m_defaultDevice = commandLinkButton->property( "webCam" ).toByteArray();
						m_selectDialog->accept();
					}
				);

				select_ui.verticalLayout->addWidget( commandLinkButton );
			}

			if ( m_selectDialog->exec() == QDialog::Rejected )
			{
				deleteLater();
				return;
			}
		}
	}


	m_camera = new QCamera( m_defaultDevice );
	connect( m_camera, SIGNAL( error( QCamera::Error ) ), this, SLOT( cameraError( QCamera::Error ) ) );
	connect( m_camera, SIGNAL( stateChanged( QCamera::State ) ), this, SLOT ( cameraStateChanged( QCamera::State ) ) );

    viewfinder = new QLabel;
    viewfinder->setMinimumSize( 50, 50 );

    m_camera->setCaptureMode( QCamera::CaptureStillImage );

    ui.gridLayout_3->addWidget( viewfinder, 0, 0 );

    m_timer = new QTimer( this );
    m_timer->setInterval( 50 );
    m_timer->start();
    connect( m_timer, &QTimer::timeout, [=]()
        {
            capture(true);
        }
    );

	show();

	m_camera->start();

    m_imageCapture = new QCameraImageCapture( m_camera );
    m_imageCapture->setCaptureDestination( QCameraImageCapture::CaptureToBuffer );

    connect(m_imageCapture, SIGNAL(imageCaptured(int,QImage)), this, SLOT(processCapturedImage(int,QImage)));
}

void webCam::processCapturedImage(int requestId, const QImage& img)
{
    if (imgOld.height()==img.height())
    {
        for (int j=0;j<img.height();j++)
        {
            QRgb*rgb1=(QRgb*)img.scanLine(j);
            QRgb*rgb2=(QRgb*)imgOld.scanLine(j);
            for (int i=0;i<img.width();i++)
            {
                QColor c=QColor(*(rgb1+i));
                int h=c.hue();
                int s=c.saturation();
                int v=c.value();
                c.setHsv(h,s,255);
                *(rgb2+i)=c.rgb();
            }
        }

        Q_UNUSED(requestId);


        viewfinder->setPixmap(QPixmap::fromImage(imgOld));
    }
    else
        imgOld=img;
}

webCam::~webCam()
{
	if ( m_camera )
	{
		m_camera->stop();
		m_camera->deleteLater();
	}

	if ( m_imageCapture )
		m_imageCapture->deleteLater();
}

void webCam::capture( bool )
{
	m_camera->searchAndLock();
    m_imageCapture->capture( );
	m_camera->unlock();
}

void webCam::cameraError( QCamera::Error value )
{
	QMessageBox::critical( this, "Error", m_camera->errorString() );

	deleteLater();
}

void webCam::cameraStateChanged( QCamera::State state )
{

}


void webCam::mousePressEvent( QMouseEvent *event )
{
	if( event->button() == Qt::LeftButton )
		m_drag_pos = event->globalPos() - frameGeometry().topLeft();
}

void webCam::mouseMoveEvent( QMouseEvent *event )
{
	if( event->buttons() & Qt::LeftButton )
		move( event->globalPos() - m_drag_pos );
}
