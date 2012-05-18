<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
<html xmlns="http://www.w3.org/1999/xhtml" xml:lang="<?php print $language->language; ?>" lang="<?php print $language->language; ?>" dir="<?php print $language->dir; ?>">
	<head>
		<title><?php print $head_title ?></title>
		<?php print $head; ?>
		<?php print $styles; ?>
		<?php print $scripts; ?>		
	</head>
	
	<body class="<?php print $body_classes; ?>">
		<div id="page">
			<table id="main-frame" cellspacing="0">
				<thead>
					<tr>
						<td class="header">
							<div>
								<?php if ($logo): ?> 
									<a href="<?php print $front_page; ?>" title="<?php print t('Home'); ?>" rel="home"><img src="<?php print $logo; ?>" alt="<?php print t('Home'); ?>" /></a>
								<?php endif; ?>
							</div>
						</td>
					</tr>
				</thead>
				<tbody>
					<tr>
						<td class="content">
							<div class="nav-bar">
								<?php if ($primary_links): ?>
		            				<div id="primary">
		              					<?php print theme('links', $primary_links); ?>
		            				</div> <!-- /#primary -->
		          				<?php endif; ?>
							</div>
							<div class="main-content">
								<?php if ($title || $tabs || $help || $messages): ?>
									<?php if ($title): ?>
								    	<h1 class="title"><?php print $title; ?></h1>
									<?php endif; ?>
									<?php print $messages; ?>
								    <?php if ($tabs): ?>
								    	<div class="tabs"><?php print $tabs; ?></div>
								    <?php endif; ?>
								    <?php print $help; ?>
								<?php endif; ?>	
								<?php print $content; ?>
							</div>
						</td>
					</tr>
				</tbody>
				<tfoot>
					<tr>
						<td class="footer">
							<div class="footer-area">
								<?php if ($footer || $footer_message): ?>
									<?php if ($footer_message): ?>
										<div class="footer-message"><?php print $footer_message; ?></div>
        							<?php endif; ?>
        						<?php print $footer; ?>
        						<?php endif; ?>
        						<div class="asce-logo"><a href="http://www.asce.org/" title="ASCE Home"><img src="/sites/all/themes/ciasce/images/asce-logo.jpg" alt="ASCE Home" /></a></div>
							</div>
						</td>
					</tr>
				</tfoot>
			</table>
		</div>
		<?php print $closure; ?>
	</body>
</html>