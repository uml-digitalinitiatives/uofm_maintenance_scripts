<?php
/**
 * @file
 * Administrative forms.
 */

function uofm_maintenance_admin_form($form, &$form_state) {
  $form = array();
  $form['batch_index_options'] = array(
    '#type' => 'fieldset',
    '#title' => t('Batch Indexer options'),
    'uofm_maintenance_batch_index_use_jms' => array(
      '#type' => 'checkbox',
      '#title' => t('Use JMS queue'),
      '#description' => t('Send items to JMS queue instead of direct to Gsearch web interface.'),
      '#default_value' => variable_get('uofm_maintenance_batch_index_use_jms', FALSE),
    ),
    'jms_options' => array(
      '#type' => 'container',
      '#states' => array(
        'visible' => array(
          ':input[name="uofm_maintenance_batch_index_use_jms"]' => array('checked' => TRUE),
        ),
        'uofm_maintenance_jms_broker' => array(
          '#type' => 'textfield',
          '#title' => t('JMS Broker address'),
          '#value' => variable_get('uofm_maintenance_jms_broker', 'tcp://localhost:61613'),
          '#size' => 35,
        ),
        'uofm_maintenance_jms_topic_queue' => array(
          '#type' => 'select',
          '#title' => t('Topic or queue?'),
          '#options' => array(
            'topic' => t('topic'),
            'queue' => t('queue'),
          ),
          '#default_value' => variable_get('uofm_maintenance_jms_topic_queue', 'topic'),
          '#description' => t('Is fedora using a topic or a queue?'),
        ),
        'uofm_maintenance_jms_topic_queue_name' => array(
          '#title' => t('Topic/queue name'),
          '#type' => 'textfield',
          '#size' => 35,
          '#value' => variable_get('uofm_maintenance_jms_topic_queue_name', 'fedora.apim.update'),
        ),
      ),
    ),
  );
  return system_settings_form($form);
}
